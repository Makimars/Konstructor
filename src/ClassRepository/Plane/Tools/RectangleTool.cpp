#include "RectangleTool.h"

RectangleTool *RectangleTool::instance = nullptr;

RectangleTool::RectangleTool(Point *mousePoint, QGraphicsScene *scene)
{
	this->objectFactory = Factory::getInstance();

}

void RectangleTool::initialize(Point *mousePoint,
							QGraphicsScene *scene,
							QBrush *defaultBrush,
							QPen *defaultPen)
{
	if(RectangleTool::instance == nullptr)
		RectangleTool::instance = new RectangleTool(mousePoint, scene);

	RectangleTool::instance->setCurrentPen(defaultPen);
	RectangleTool::instance->setCurrentBrush(defaultBrush);
}

RectangleTool *RectangleTool::getInstance()
{
	return RectangleTool::instance;
}

void RectangleTool::click(DrawableObject *clickedObject, Point *mousePoint)
{

}

void RectangleTool::resetTool()
{

}
