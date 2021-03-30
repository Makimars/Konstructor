#include "LabelTool.h"

LabelTool *LabelTool::instance = nullptr;

LabelTool::LabelTool() : Tool()
{
	toolTips.append(tr("Place label."));
}

LabelTool *LabelTool::getInstance()
{
	if(LabelTool::instance == nullptr)
		LabelTool::instance = new LabelTool();
	return LabelTool::instance;
}

void LabelTool::click(DrawableObject *clickedObject, QPointF pos)
{
	Label *label = this->planeFactory->makeLabel(pos);
	this->planeFactory->addDrawable(label);
	label->requestText();
}
