#include "DimensionTool.h"

DimensionTool *DimensionTool::instance = nullptr;

DimensionTool::DimensionTool(Point *mouse_point, QGraphicsScene *scene)
{

}

void DimensionTool::initialise(Point *mouse_point,
							QGraphicsScene *scene,
							QBrush *default_brush,
							QPen *default_pen)
{
	if(DimensionTool::instance == nullptr)
		DimensionTool::instance = new DimensionTool(mouse_point, scene);

	DimensionTool::instance->setCurrentPen(default_pen);
	DimensionTool::instance->setCurrentBrush(default_brush);
}

DimensionTool *DimensionTool::getInstance()
{
	return DimensionTool::instance;
}

void DimensionTool::click(DrawableObject *clicked_object, Point *mouse_point)
{
	this->clicked_objects[1] = this->clicked_objects[0];
	this->clicked_objects[0] = clicked_object;

	if(this->clicked_objects[1] != nullptr)
	{
		QString object_0_type = this->clicked_objects[0]->getType();
		QString object_1_type = this->clicked_objects[1]->getType();

		if(object_0_type == TYPE_LINE & object_1_type == TYPE_LINE)
		{
			// two lines
			Line *lines[] = {
				dynamic_cast<Line*>(this->clicked_objects[0]),
				dynamic_cast<Line*>(this->clicked_objects[1])
			};

			// one common point
			if((lines[0]->getStartPoint() == lines[1]->getStartPoint()) |
					(lines[0]->getStartPoint() == lines[1]->getEndPoint()))
			{
				//two lines angle
			}
			else // no common point
			{
				//two lines distance
			}
		}
		else if((object_0_type == TYPE_POINT | object_1_type == TYPE_POINT) &
				(object_0_type == TYPE_LINE  | object_1_type == TYPE_LINE))
		{
			//one line and one point



		}

	}


}

void DimensionTool::resetTool()
{
	this->object_factory->tryDeleteDrawable(this->clicked_objects[0]);
	this->clicked_objects[0] = nullptr;

	this->object_factory->tryDeleteDrawable(this->clicked_objects[1]);
	this->clicked_objects[1] = nullptr;
}
