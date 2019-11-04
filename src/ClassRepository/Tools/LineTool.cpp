#include "LineTool.h"

LineTool *LineTool::instance = nullptr;

LineTool::LineTool(Point *mouse_point, QGraphicsScene *scene)
{
	this->object_factory = DrawablesFactory::getInstance();

	this->line_preview_start_point = this->object_factory->makePoint();
	this->line_preview = this->object_factory
			->makeLine(this->line_preview_start_point, mouse_point);
	this->line_preview->setHidden(true);
	scene->addItem(this->line_preview);
}

void LineTool::initialise(Point *mouse_point,
							QGraphicsScene *scene,
							QBrush *default_brush,
							QPen *default_pen)
{
	if(LineTool::instance == nullptr)
		LineTool::instance = new LineTool(mouse_point, scene);

	LineTool::instance->setCurrentPen(default_pen);
	LineTool::instance->setCurrentBrush(default_brush);
}

LineTool *LineTool::getInstance()
{
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
			this->object_factory->addDrawable(this->clicked_points[0]);
		this->object_factory->addDrawable(this->clicked_points[1]);

		Line *line = this->object_factory
				->makeLine(this->clicked_points[0], this->clicked_points[1]);
		this->object_factory->addDrawable(line);

		this->clicked_points[1] = nullptr;
	}
}

void LineTool::resetTool()
{
	this->object_factory->tryDeleteDrawable(this->clicked_points[0]);
	this->clicked_points[0] = nullptr;

	this->object_factory->tryDeleteDrawable(this->clicked_points[1]);
	this->clicked_points[1] = nullptr;

	this->line_preview->setHidden(true);
}
