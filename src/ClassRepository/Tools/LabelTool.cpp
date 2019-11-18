#include "LabelTool.h"

LabelTool *LabelTool::instance = nullptr;

LabelTool::LabelTool(Point *mouse_point, QGraphicsScene *scene)
{
	this->object_factory = DrawablesFactory::getInstance();
}

void LabelTool::initialise(Point *mouse_point,
							QGraphicsScene *scene,
							QBrush *default_brush,
							QPen *default_pen)
{
	if(LabelTool::instance == nullptr)
		LabelTool::instance = new LabelTool(mouse_point, scene);

	LabelTool::instance->setCurrentPen(default_pen);
	LabelTool::instance->setCurrentBrush(default_brush);
}

LabelTool *LabelTool::getInstance()
{
	return LabelTool::instance;
}

void LabelTool::click(DrawableObject *clicked_object, Point *mouse_point)
{

}

void LabelTool::resetTool()
{

}
