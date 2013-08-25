#pragma once

#include <logicalModelAssistInterface.h>
#include <graphUtils/deepFirstSearcher.h>

#include "generatorCustomizer.h"

namespace qReal {
namespace robots {
namespace generators {

class RobotsDiagramVisitor : public utils::DeepFirstSearcher::VisitorInterface
{
public:
	RobotsDiagramVisitor(LogicalModelAssistInterface const &model
			, GeneratorCustomizer const &customizer);
	virtual ~RobotsDiagramVisitor();

protected:
	// TODO: move this enum to global robots difinitions and use it
	enum LinkGuard
	{
		emptyGuard = 0
		, iterationGuard
		, trueGuard
		, falseGuard
	};

	void startSearch(Id const &startingBlock);
	void terminateSearch();

	LinkGuard guardOf(Id const &linkId) const;

	virtual void visitRegular(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;
	virtual void visitFinal(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;
	virtual void visitConditional(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;
	virtual void visitLoop(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;
	virtual void visitSwitch(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;
	virtual void visitFork(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;
	virtual void visitUnknown(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links);

private:
	virtual void visit(Id const &nodeId, QList<utils::DeepFirstSearcher::LinkInfo> const &links);

	LogicalModelAssistInterface const &mModel;
	GeneratorCustomizer const &mCustomizer;
	utils::DeepFirstSearcher mDfser;
};

}
}
}
