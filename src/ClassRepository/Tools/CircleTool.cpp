#include "CircleTool.h"

CircleTool *CircleTool::instance = nullptr;

CircleTool::CircleTool(Point *mouse_point, QGraphicsScene *scene)
{
	this->circle_preview_centre = new Point(0,0);
	this->circle_preview = new Circle(this->circle_preview_centre, mouse_point);
	this->circle_preview->setHidden(true);

	scene->addItem(this->circle_preview);
	this->previous_clicked_point = nullptr;
}

CircleTool *CircleTool::getInstance(Point *mouse_point, QGraphicsScene *scene)
{
	if(CircleTool::instance == nullptr)
		CircleTool::instance = new CircleTool(mouse_point, scene);

	return CircleTool::instance;
}

void CircleTool::click(Point *clicked_point, bool cloned_point)
{

	if(this->previous_clicked_point != nullptr)
	{
		emit addDrawable(this->previous_clicked_point);
		if(cloned_point)
		{
			addCircle(this->previous_clicked_point,
						this->previous_clicked_point->distanceFrom(clicked_point->getLocation())
						);
			emit tryDeleteDrawable(clicked_point);
		}
		else
		{
			addCircle(this->previous_clicked_point, clicked_point);
		}

		this->circle_preview->setHidden(true);
		this->previous_clicked_point = nullptr;
	}
	else
	{
		this->previous_clicked_point = clicked_point;
		this->circle_preview_centre->setLocation(clicked_point->getLocation());
		this->circle_preview->setHidden(false);
	}
}

void CircleTool::resetTool()
{
	emit tryDeleteDrawable(this->previous_clicked_point);
	this->circle_preview->setHidden(true);
}
