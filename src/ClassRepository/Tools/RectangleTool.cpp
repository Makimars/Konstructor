#include "RectangleTool.h"

RectangleTool *RectangleTool::instance = nullptr;

RectangleTool::RectangleTool(Point *mouse_point, QGraphicsScene *scene)
{
	this->object_factory = DrawablesFactory::getInstance();

}

void RectangleTool::initialise(Point *mouse_point,
							QGraphicsScene *scene,
							QBrush *default_brush,
							QPen *default_pen)
{
	if(RectangleTool::instance == nullptr)
		RectangleTool::instance = new RectangleTool(mouse_point, scene);

	RectangleTool::instance->setCurrentPen(default_pen);
	RectangleTool::instance->setCurrentBrush(default_brush);
}

RectangleTool *RectangleTool::getInstance()
{
	return RectangleTool::instance;
}

void RectangleTool::click(DrawableObject *clicked_object, Point *mouse_point)
{

}

void RectangleTool::resetTool()
{

}
