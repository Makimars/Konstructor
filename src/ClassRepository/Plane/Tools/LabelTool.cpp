#include "LabelTool.h"

LabelTool *LabelTool::instance = nullptr;

LabelTool::LabelTool() : Tool()
{
}

LabelTool *LabelTool::getInstance()
{
	if(LabelTool::instance == nullptr)
		LabelTool::instance = new LabelTool();
	return LabelTool::instance;
}

void LabelTool::click(DrawableObject *clickedObject, QPointF pos)
{
	Label *label = this->objectFactory->makeLabel(mousePoint->getLocation());
	this->objectFactory->addDrawable(label);
	label->requestText();
}

void LabelTool::resetTool()
{

}
