#include "LabelTool.h"

LabelTool *LabelTool::instance = nullptr;

LabelTool::LabelTool(Point *mousePoint, QGraphicsScene *scene)
{
	this->objectFactory = DrawablesFactory::getInstance();
}

void LabelTool::initialize(Point *mousePoint,
							QGraphicsScene *scene,
							QBrush *defaultBrush,
							QPen *defaultPen)
{
	if(LabelTool::instance == nullptr)
		LabelTool::instance = new LabelTool(mousePoint, scene);

	LabelTool::instance->setCurrentPen(defaultPen);
	LabelTool::instance->setCurrentBrush(defaultBrush);
}

LabelTool *LabelTool::getInstance()
{
	return LabelTool::instance;
}

void LabelTool::click(DrawableObject *clickedObject, Point *mousePoint)
{
	Label *label = this->objectFactory->makeLabel(mousePoint->getLocation());
	this->objectFactory->addDrawable(label);
	label->requestText();
}

void LabelTool::resetTool()
{

}
