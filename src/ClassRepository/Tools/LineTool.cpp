#include "LineTool.h"

LineTool *LineTool::instance = nullptr;

LineTool::LineTool(Point *mouse_point, QGraphicsScene *scene)
{
	this->line_preview_start_point = new Point();
	this->line_preview = new Line(this->line_preview_start_point, mouse_point);
	this->line_preview->setHidden(true);
	scene->addItem(this->line_preview);
}

LineTool *LineTool::getInstance(Point *mouse_point, QGraphicsScene *scene)
{
	if(LineTool::instance == nullptr)
		LineTool::instance = new LineTool(mouse_point, scene);

	return LineTool::instance;
}

void LineTool::click(Point *clicked_point, bool existing_point)
{
	this->clicked_points[1] = this->clicked_points[0];
	this->clicked_points[0] = clicked_point;

	this->line_preview->setHidden(false);
	this->line_preview_start_point->setLocation(this->clicked_points[0]->getLocation());

	if(this->clicked_points[1] != nullptr)
	{
		if(!existing_point)
			emit addDrawable(this->clicked_points[0]);
		emit addDrawable(this->clicked_points[1]);
		addLine(this->clicked_points[0], this->clicked_points[1]);

		this->clicked_points[1] = nullptr;
	}
}

void LineTool::resetTool()
{
	emit tryDeleteDrawable(this->clicked_points[0]);
	this->clicked_points[0] = nullptr;
	emit tryDeleteDrawable(this->clicked_points[1]);
	this->clicked_points[1] = nullptr;
	this->line_preview->setHidden(true);
}
