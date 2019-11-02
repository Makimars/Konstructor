#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include "LineTool.h"

class CircleTool : public Tool
{
public:
	static CircleTool *getInstance(Point *mouse_point, QGraphicsScene *scene);

	void click(Point *clicked_point, bool existing_point = false) override;
	void resetTool() override;

private:
	CircleTool(Point *mouse_point, QGraphicsScene *scene);
	static CircleTool *instance;

	//working variables
	Point *previous_clicked_point;

	//preview
	Circle *circle_preview;
	Point *circle_preview_centre;
};

#endif // CIRCLETOOL_H
