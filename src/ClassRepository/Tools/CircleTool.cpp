#include "CircleTool.h"

CircleTool *CircleTool::instance = nullptr;

CircleTool::CircleTool(Point *mouse_point, QGraphicsScene *scene)
{
	this->object_factory = DrawablesFactory::getInstance();

	this->circle_preview_centre = this->object_factory->makePoint();
	this->circle_preview = this->object_factory
			->makeCircle(this->circle_preview_centre, mouse_point);
	this->circle_preview->setHidden(true);

	scene->addItem(this->circle_preview);
	this->previous_clicked_point = nullptr;
}

void CircleTool::initialise(Point *mouse_point,
							QGraphicsScene *scene,
							QBrush *default_brush,
							QPen *default_pen)
{
	if(CircleTool::instance == nullptr)
		CircleTool::instance = new CircleTool(mouse_point, scene);

	CircleTool::instance->setCurrentPen(default_pen);
	CircleTool::instance->setCurrentBrush(default_brush);
}

CircleTool *CircleTool::getInstance()
{
	return CircleTool::instance;
}

void CircleTool::click(DrawableObject *clicked_object, Point *mouse_point)
{
	if(this->previous_clicked_point != nullptr)
	{
		this->object_factory->addDrawable(this->previous_clicked_point);

		if(clicked_object == nullptr)
		{
			Point *clicked_point = mouse_point->Clone();
			Circle *circle = this->object_factory
									->makeCircle(this->previous_clicked_point,
												this->previous_clicked_point->distanceFrom(clicked_point->getLocation())
												);
			this->object_factory->addDrawable(circle);

			this->object_factory->tryDeleteDrawable(clicked_point);
		}
		else
		{
			Circle *circle = this->object_factory
									->makeCircle(this->previous_clicked_point,
												 clicked_object
												 );
			this->object_factory->addDrawable(circle);
		}

		this->circle_preview->setHidden(true);
		this->previous_clicked_point = nullptr;
	}
	else
	{
		if(clicked_object == nullptr)
			clicked_object = mouse_point->Clone();

		if(clicked_object->getType() == TYPE_POINT)
		{
			Point *clicked_point = dynamic_cast<Point*>(clicked_object);

			this->previous_clicked_point = clicked_point;
			this->circle_preview_centre->setLocation(clicked_point->getLocation());
			this->circle_preview->setHidden(false);
		}

	}
}

void CircleTool::resetTool()
{
	this->object_factory->tryDeleteDrawable(this->previous_clicked_point);
	this->circle_preview->setHidden(true);
}
