/* Copyright 2017 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "pioneerStateMachineGenerator.h"

#include <generatorBase/semanticTree/semanticNode.h>
#include <generatorBase/semanticTree/simpleNode.h>

#define TRACE

using namespace pioneer::lua;
using namespace generatorBase;
using namespace generatorBase::semantics;

PioneerStateMachineGenerator::PioneerStateMachineGenerator(
		const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, generatorBase::GeneratorCustomizer &customizer
		, generatorBase::PrimaryControlFlowValidator &validator
		, const qReal::Id &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: GotoControlFlowGenerator(repo, errorReporter, customizer, validator, diagramId, parent, isThisDiagramMain)
	, mConditionals(0)
	, mConditionalEnds(0)
{
	mAsynchronousNodes << "GeoTakeoff" << "GeoLanding" << "GoToPoint" << "GoToGPSPoint";
}

void PioneerStateMachineGenerator::registerNodeHook(std::function<void(const qReal::Id)> hook)
{
	mNodeHooks.append(hook);
}

void PioneerStateMachineGenerator::performGeneration()
{
	mConditionals = mConditionalEnds = 0;
	mConditionZonesQueue.clear();
	mSemanticTreeManager.reset(new SemanticTreeManager(*mSemanticTree, mErrorReporter, mErrorsOccured));
	GotoControlFlowGenerator::performGeneration();
	if (mConditionals != mConditionalEnds && not errorsOccured()) {
		reportError(tr("The diagram must have the same number of \"Conditonal\" and \"End If\" blocks."));
	}
}

void PioneerStateMachineGenerator::visitRegular(const qReal::Id &id, const QList<LinkInfo> &links)
{
	// Base class method checks for subprogram calls, which is irrelevant for now, but does not hurt and hopefully
	// will be needed later.
	ControlFlowGeneratorBase::visitRegular(id, links);

	if (mErrorsOccured) {
		return;
	}

	if (id.element() == "InitialNode") {
		mLabeledNodes << links.first().target;
	}

	if (not mVisitedNodes.contains(id) && id.element() == "FiBlock") {
		++mConditionalEnds;
	}

	trace("Visiting " + id.toString());

	const qReal::Id target = links[0].target;
	QList<NonZoneNode *> nodesWithThisId = mSemanticTreeManager->nodes(id);
	for (auto thisNode : nodesWithThisId) {
		if (thisNode) {
			processNode(thisNode, target);
		} else {
			mErrorsOccured = true;
			return;
		}
	}

	if (mErrorsOccured) {
		return;
	}

	// Generation of a node may lead to sudden appearance of new copies of a node (for example, if this node was in a
	// branch of If statement that get copied when generating node, quite common in if-loop programs). So we need to
	// process new clones as well.
	auto updatedNodes = mSemanticTreeManager->nodes(id);
	while (updatedNodes.size() != nodesWithThisId.size()) {
		for (auto node : updatedNodes) {
			if (!nodesWithThisId.contains(node)) {
				processNode(node, target);
				nodesWithThisId.append(node);
			}
		}

		updatedNodes = mSemanticTreeManager->nodes(id);
	}

	doDeferredGotoGeneration(id, target);
}

void PioneerStateMachineGenerator::processNode(NonZoneNode *thisNode, const qReal::Id &target)
{
	NonZoneNode *nextNode = nullptr;

	if (mAsynchronousNodes.contains(thisNode->id().element())) {
		if (not mSemanticTreeManager->isTopLevelNode(thisNode)) {
			// we in the if branches or smth like that
			if (not mConditionZonesQueue.isEmpty()) {
				const auto * const thenZone = std::get<2>(mConditionZonesQueue.last());
				const auto * const elseZone = std::get<3>(mConditionZonesQueue.last());
				auto * const parentZone = thisNode->parentZone();
				if (thenZone == parentZone || elseZone == parentZone) {
					mBranchAsyncMarkers[parentZone] = true;
				}
			}
		}

		if (mSemanticTree->findNodeFor(target)) {
			trace("Asynchronous node, target visited.");

			// thisNode is asyncronous node that transfers control to already visited node.
			// Generated code for thisNode will initiate asynchronous action and all we need to do is to generate
			// transition to a state which will execute target block when this block finishes its asynchronous
			// operation.
			auto rightSibling = mSemanticTreeManager->anyRightSibling(thisNode);
			if (!rightSibling || !mSemanticTreeManager->isGotoNode(rightSibling)) {
				// Goto node may already be produced as a copy from previous instance of this node, so skipping it if
				// not needed.
				nextNode = produceGotoNode(target);
				mSemanticTreeManager->addAfter(thisNode, nextNode);
			}

			// already visited target
			if (nextNode->id().element() == "FiBlock") {
				if (not mConditionZonesQueue.isEmpty()) {
					if (std::get<1>(mConditionZonesQueue.last())) {
						mConditionZonesQueue.removeLast();
					} else {
						reportAndExplainConditions();
						return;
					}
				} else {
					reportError(tr("\"End If\" block occurs before \"If block\""));
					return;
				}
			}

			if (!mLabeledNodes.contains(target)) {
				// Target node, despite being already visited, does not have a label, it means that it is a part of
				// a synchronous fragment. We copy that fragment from this node to the first asyncronous node and
				// label a start of the copied fragment. At the end of the fragment we will generate said
				// asynchronous node, which will initiate asynchronous operation, and then transition to its next
				// state, which will continue execution when operation is done.
				//
				// But first we need to lift copied fragment to a top level, since nextNode may be inside If branch.
				// Labeling something inside If branch will make generated code uncompilable.
				//
				// Getting parent node (i.e. If statement to the branch of which our node belongs).
				while (!mSemanticTreeManager->isTopLevelNode(nextNode)) {
					nextNode = mSemanticTreeManager->parent(nextNode);
				}

				// We shall copy nodes from synchronous fragment after end-of-handler node, if it is present.
				auto sibling = mSemanticTreeManager->anyRightSibling(nextNode);
				while (sibling != nullptr && isEndOfHandler(sibling)) {
					nextNode = dynamic_cast<NonZoneNode*>(mSemanticTreeManager->anyRightSibling(nextNode));
					sibling = mSemanticTreeManager->anyRightSibling(nextNode);
				}

				nextNode = copySynchronousFragment(nextNode, target, true);
			}

			if (mSemanticTreeManager->isTopLevelNode(thisNode)
						&& !isEndOfHandler(nextNode)
						&& !isEndOfHandler(mSemanticTreeManager->anyRightSibling(nextNode))) {
				SemanticNode * const endNode = produceEndOfHandlerNode();
				mSemanticTreeManager->addAfter(nextNode, endNode);
			}
		} else {
			trace("Asynchronous node, target not visited.");

			// thisNode is asynchronous node that transfers control to a node that has not been visited yet. Generating
			// transition into a state associated with that node and then a new handler for target node itself.
			nextNode = mSemanticTreeManager->produceLabeledNode(target);
			if (!nextNode) {
				reportError(tr("Generation internal error, failed to create a node."));
				return;
			} else {
				mLabeledNodes << nextNode->id();
			}

			SemanticNode * const gotoNode = produceGotoNode(target);
			mSemanticTreeManager->addAfter(thisNode, gotoNode);

			// Labeled node can not be a part of a zone (i.e. "then" or "else" branch), it shall be generated in top
			// level zone.
			if (mSemanticTreeManager->isTopLevelNode(thisNode)) {
				SemanticNode * const endNode = produceEndOfHandlerNode();
				mSemanticTreeManager->addAfter(gotoNode, endNode);
				mSemanticTreeManager->addAfter(endNode, nextNode);
			} else {
				// Getting parent node (i.e. If statement to the branch of which our node belongs).
				NonZoneNode *parent = mSemanticTreeManager->topLevelParent(thisNode);

				// Skipping "end" that finishes handler with If.
				SemanticNode * endOfHandler = findEndOfHandler(parent);
				if (not endOfHandler) {
					endOfHandler = produceEndOfHandlerNode();
					mSemanticTreeManager->addAfter(parent, endOfHandler);
				}

				// Adding our labeled node denoting new handler after the end of a handler with If node.
				mSemanticTreeManager->addAfter(endOfHandler, nextNode);
			}

			// here we first time visiting FiBlock and it is descended from async node
			// it can be not actually in branch zone anymore (if async block appear before it)
			if (nextNode->id().element() == "FiBlock") {
				std::get<1>(mConditionZonesQueue.last()) = true;
			}
		}
	} else {
		if (!mSemanticTree->findNodeFor(target)) {
			trace("Synchronous node, target not visited.");
			if (target.element() == "FiBlock") {
				nextNode = mSemanticTreeManager->produceNode(target);
				if (mConditionZonesQueue.isEmpty()) {
					reportError(tr("\"End If\" block occurs before \"If block\""));
				} else {
					auto conditionZone = mConditionZonesQueue.last();
					SemanticNode *conditionalZone = std::get<0>(conditionZone);
					mSemanticTreeManager->addAfter(conditionalZone, nextNode);
				}

				return;
			}

			// It is not an asynchronous node, generating as-is.
			nextNode = mSemanticTreeManager->produceNode(target);
			mSemanticTreeManager->addAfter(thisNode, nextNode);
		} else {
			trace("Synchronous node, target visited.");

			if (target.element() == "FiBlock") {
				auto thenZone = std::get<2>(mConditionZonesQueue.last());
				auto elseZone = std::get<3>(mConditionZonesQueue.last());
				if (std::get<1>(mConditionZonesQueue.last())
						|| mBranchAsyncMarkers[thenZone] || mBranchAsyncMarkers[elseZone]) {
					reportAndExplainConditions();
				} else {
					mConditionZonesQueue.removeLast();
					nextNode = mSemanticTreeManager->produceNode(target);
				}

				return;
			} else {
				// Synchronous node leading to already visited node. Need some copypasting of synchronous fragments,
				// or else we will stall the program waiting for an event that was never initiated.
				nextNode = copySynchronousFragment(thisNode, target, false);
			}

			if (mSemanticTreeManager->isTopLevelNode(thisNode) && nextNode && !isEndOfHandler(nextNode)) {
				SemanticNode * const endNode = produceEndOfHandlerNode();
				mSemanticTreeManager->addAfter(nextNode, endNode);
			}
		}
	}
}

void PioneerStateMachineGenerator::visitConditional(const qReal::Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	trace("Visiting conditional node: " + id.toString());
	if (mErrorsOccured) {
		return;
	}

	if (not mVisitedNodes.contains(id)) {
		++mConditionals;
	}

	const QPair<LinkInfo, LinkInfo> branches(ifBranchesFor(id));
	const LinkInfo thenLink = branches.first;
	const LinkInfo elseLink = branches.second;

	const auto nodes = mSemanticTreeManager->nodes(id);

	for (const auto node : nodes) {
		IfNode * const thisNode = static_cast<IfNode *>(node);

		mSemanticTreeManager->addToZone(thisNode->thenZone(), thenLink.target);
		mSemanticTreeManager->addToZone(thisNode->elseZone(), elseLink.target);
		mConditionZonesQueue.enqueue(std::make_tuple(thisNode, false, thisNode->thenZone(), thisNode->elseZone()));
		mBranchAsyncMarkers[thisNode->thenZone()] = false;
		mBranchAsyncMarkers[thisNode->elseZone()] = false;

		if (!mSemanticTreeManager->isTopLevelNode(thisNode)) {
			reportError(tr("Nested If's constructions is not allowed."));
			return;
		}
	}
}

void PioneerStateMachineGenerator::visitFinal(const qReal::Id &id, const QList<LinkInfo> &links)
{
	generatorBase::GotoControlFlowGenerator::visitFinal(id, links);

	trace("Visiting final node: " + id.toString());
	if (mErrorsOccured) {
		return;
	}

	// Here we are going to add finishing end-of-handler node in case it is missing (for example, diagrams like
	// "Initial Node" -> "Final Node" will not generate it automatically).
	// It is a kind of hack because asynchronous handler shall be a first-class entity and a zone node.

	auto nodes = mSemanticTreeManager->nodes(id);
	for (auto node : nodes) {
		if (!node) {
			continue;
		}

		auto thisNode = node;
		auto parent = mSemanticTreeManager->topLevelParent(thisNode);
		auto endOfHandler = findEndOfHandler(parent);

		if (!endOfHandler) {
			// If not found, create and add one.
			endOfHandler = produceEndOfHandlerNode();
			mSemanticTreeManager->addAfter(thisNode, endOfHandler);
		}
	}
}

void PioneerStateMachineGenerator::visit(const qReal::Id &nodeId, QList<utils::DeepFirstSearcher::LinkInfo> &links)
{
	if (mVisitedNodes.contains(nodeId) || mErrorsOccured) {
		return;
	}

	generatorBase::GotoControlFlowGenerator::visit(nodeId, links);
	for (const auto &hook : mNodeHooks) {
		hook(nodeId);
	}

	mVisitedNodes.insert(nodeId);
}

NonZoneNode *PioneerStateMachineGenerator::copySynchronousFragment(NonZoneNode *after
		, const qReal::Id &from
		, bool withLabel)
{
	trace("Copying synchronous fragment from "
			+ from.toString()
			+ " after "
			+ after->id().toString()
			+ " with label = "
			+ (withLabel ? "true" : "false"));

	// Here "from" may have many corresponding nodes in a semantic tree, but any node will do, so using findNodeFor.
	NonZoneNode *oldFragmentStart = dynamic_cast<NonZoneNode *>(mSemanticTree->findNodeFor(from));
	if (!oldFragmentStart) {
		reportError(tr("Generation internal error, zone node corresponds to a block in a diagram."));
		return nullptr;
	}

	NonZoneNode *fragmentStartNode = withLabel
			? mSemanticTreeManager->produceLabeledNode(from)
			: mSemanticTreeManager->produceNode(from);

	if (!fragmentStartNode) {
		return nullptr;
	} else {
		mLabeledNodes << fragmentStartNode->id();
	}

	if (!dynamic_cast<NonZoneNode *>(after)) {
		reportError(tr("Generation internal error, non-zone node is a start of a fragment."));
		return nullptr;
	}

	if (withLabel && !isIf(after) && !isEndOfHandler(after)) {
		// End-of-handler shall go before every labeled node, since label here is actually a start of a new handler.
		// "If" nodes have their own end-of-handler, so they shall not be generated.
		SemanticNode * const endNode = produceEndOfHandlerNode();
		mSemanticTreeManager->addAfter(after, endNode);
		mSemanticTreeManager->addAfter(endNode, fragmentStartNode);
	} else {
		mSemanticTreeManager->addAfter(after, fragmentStartNode);
	}

	if (isAsynchronous(fragmentStartNode)) {
		// Synchronous fragment is trivial and its first node is asynchronous. Generating transition from it and we're
		// done here.
		//
		// Using oldFragmentStart because fragmentStartNode was just added and does not have siblings, but it is a copy
		// of oldFragmentStart.
		auto rightSibling = mSemanticTreeManager->nonSyntheticRightSibling(oldFragmentStart);
		if (rightSibling) {
			if (!isIf(fragmentStartNode)) {
				const auto gotoNode = produceGotoNode(rightSibling->id());
				fragmentStartNode->appendSibling(gotoNode);
				return gotoNode;
			} else {
				// This If node is copied without its end-of-handler node, so we need to check for endNode ourselves.
				if (mSemanticTreeManager->isTopLevelNode(fragmentStartNode)) {
					auto endNode = produceEndOfHandlerNode();
					mSemanticTreeManager->addAfter(fragmentStartNode, endNode);
					return endNode;
				}

				return fragmentStartNode;
			}
		} else {
			// Target of fragment start was not visited yet. Here we don't know yet where to generate a goto, so we
			// need to defer generation --- save Id of a fragment start and a node after which we shall add "goto"
			// when we will know where it leads.
			mDeferredGotoNodes.insert(fragmentStartNode->id(), fragmentStartNode);
			return fragmentStartNode;
		}
	}

	auto siblings = mSemanticTreeManager->copyRightSiblingsUntil(
			oldFragmentStart
			, [this](SemanticNode * node){ return isAsynchronous(node); });

	if (siblings.isEmpty()) {
		// Fragment is trivial and non-asynchronous --- so it must be FinalNode. Fine, no need to copy it.
		if (fragmentStartNode->id().element() != "FinalNode") {
			reportError(tr("Generation internal error, program ends abruptly."));
			return nullptr;
		}

		return fragmentStartNode;
	}

	if (isAsynchronous(siblings.last())) {
		// Synchronous fragment finished with asynchronous node
		fragmentStartNode->appendSiblings(siblings);

		// Now we shall look for the end of original fragment: find asynchronous node on which a fragment shall be
		// ending and get its target node. If it is a simple loop, asynchronous node shall be already visited, and
		// shall have one target node. If it is an If statement, If already can take care of transferring control
		// through asynchronous nodes in its branches.
		auto asynchronousNode = mSemanticTreeManager->findSibling(
				oldFragmentStart
				, [this](SemanticNode * node){ return isAsynchronous(node); });

		// If this node is "If" with asynchronous branches, it will take care of itself, otherwise need to generate
		// Goto node pointing to a target of asynchronous node, if we were visited it already. If not, it will generate
		// Goto by itself.
		if (!isIf(asynchronousNode)) {
			auto lastSiblingNonZoneNode = dynamic_cast<NonZoneNode *>(siblings.last());
			if (mVisitedNodes.contains(lastSiblingNonZoneNode->id())) {
				auto asynchronousNodeTarget = mSemanticTreeManager->nonSyntheticRightSibling(asynchronousNode);
				if (!asynchronousNodeTarget) {
					reportError(tr("Generation internal error, asynchronous node does not have target node."));
					return nullptr;
				}
				auto gotoNode = produceGotoNode(asynchronousNodeTarget->id());
				lastSiblingNonZoneNode->appendSibling(gotoNode);
				return gotoNode;
			} else {
				return lastSiblingNonZoneNode;
			}
		} else {
			auto ifNode = dynamic_cast<IfNode *>(siblings.last());
			// This If node is copied without its end-of-handler node, so we need to check for endNode ourselves.
			if (mSemanticTreeManager->isTopLevelNode(ifNode)) {
				auto endNode = produceEndOfHandlerNode();
				mSemanticTreeManager->addAfter(ifNode, endNode);
				return endNode;
			}

			return ifNode;
		}
	} else {
		reportError(tr("There is a problem with If construction or with loops"
				" (loops without sending requests to the autopilot"
				" through \"Takeoff\", \"Landing\", \"Go to local point\" blocks are not supported yet)."));
	}

	return nullptr;
}

bool PioneerStateMachineGenerator::isAsynchronous(const SemanticNode * const node) const
{
	// A node is asynchronous if it itself is asynchronous or if it is If node and its branches are asynchronous.
	if (mAsynchronousNodes.contains(node->id().element())) {
		return true;
	}

	if (isIf(node)) {
		// Do not bothering to look inside If branches, it does not support synchronous branches anyway and will produce
		// error message by itself in such case.
		return true;
	}

	return false;
}

SimpleNode *PioneerStateMachineGenerator::produceEndOfHandlerNode()
{
	trace("End-of-handler");
	qReal::Id syntheticId = qReal::Id::createElementId("synthetic", "synthetic", "EndOfHandler");
	SimpleNode * const result = mSemanticTree->produceSimple(syntheticId);
	// No need for special handling, from the point of view of a generator it is just some simple node.
	result->bindToSyntheticConstruction(SimpleNode::noSytheticBinding);
	return result;
}

void PioneerStateMachineGenerator::addInfo(const QString &message) const
{
	mErrorReporter.addInformation(message);
}

void PioneerStateMachineGenerator::reportAndExplainConditions()
{
	addInfo(tr("The blocks (\"Takeoff\", \"Landing\", \"Go to local point\") which work with "
			"autopilot were observed in both (or only in one) conditional branches."));
	reportError(tr("Such blocks must appear and finish both branches "
			"(the \"End if\" block must have two such parents) or not appear at branches at all."));
	addInfo(tr("Such blocks may appear in each"
			" branch several times but one of them must finish it in each branch."));
}

void PioneerStateMachineGenerator::reportError(const QString &message)
{
	trace("ERROR OCCURRED!!!");
	mErrorReporter.addError(message);
	mErrorsOccured = true;
}

bool PioneerStateMachineGenerator::isIf(const SemanticNode * const node)
{
	return dynamic_cast<IfNode const * const>(node) != nullptr;
}

bool PioneerStateMachineGenerator::isEndOfHandler(const SemanticNode * const node)
{
	if (!node) {
		return false;
	}

	return node->id().element() == "EndOfHandler";
}

NonZoneNode *PioneerStateMachineGenerator::findEndOfHandler(SemanticNode * const from) const
{
	return dynamic_cast<NonZoneNode *>(
			mSemanticTreeManager->findSibling(from, [](SemanticNode *node){ return isEndOfHandler(node); })
	);
}

void PioneerStateMachineGenerator::doDeferredGotoGeneration(const qReal::Id &nodeId, const qReal::Id &targetId)
{
	if (mDeferredGotoNodes.contains(nodeId)) {
		QList<NonZoneNode *> nodesToCheck;
		for (auto node : mDeferredGotoNodes.values(nodeId)) {
			nodesToCheck << node;
			nodesToCheck << mSemanticTreeManager->clones(node);
		}

		for (auto node : nodesToCheck) {
			const auto sibling = mSemanticTreeManager->anyRightSibling(node);
			if (!sibling || !mSemanticTreeManager->isGotoNode(sibling)) {
				auto gotoNode = produceGotoNode(targetId);
				mSemanticTreeManager->addAfter(node, gotoNode);
			}
		}

		mDeferredGotoNodes.remove(nodeId);
	}
}

void PioneerStateMachineGenerator::trace(const QString &message)
{
	Q_UNUSED(message)
}
