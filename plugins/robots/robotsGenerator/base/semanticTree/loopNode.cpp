#include "loopNode.h"

using namespace qReal::robots::generators::semantics;

LoopNode::LoopNode(Id const &idBinded, QObject *parent)
	: ConditionalNode(idBinded, parent)
	, mBodyZone(new ZoneNode(this))
	, mDoWhileForm(false)
{
	mBodyZone->setParentNode(this);
}

QString LoopNode::toString() const
{
	return QString(mDoWhileForm ? "DO-WHILE " : "while-do ") + ConditionalNode::toString();
}

void LoopNode::appendChildren(QLinkedList<SemanticNode *> const &nodes)
{
	mBodyZone->appendChildren(nodes);
}

void LoopNode::setForm(bool doWhileForm)
{
	mDoWhileForm = doWhileForm;
}

ZoneNode *LoopNode::bodyZone()
{
	return mBodyZone;
}

QLinkedList<SemanticNode *> LoopNode::children() const
{
	return QLinkedList<SemanticNode *>() << mBodyZone;
}
