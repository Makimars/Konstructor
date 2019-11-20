#include "LabelTool.h"

LabelTool *LabelTool::instance = nullptr;

LabelTool::LabelTool(Point *mousePoint, QGraphicsScene *scene)
{
	this->objectFactory = DrawablesFactory::getInstance();
}

void LabelTool::initialise(Point *mousePoint,
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

}

void LabelTool::resetTool()
{

}
