#ifndef LINETOOL_H
#define LINETOOL_H

#include "Tool.h"

class LineTool : public Tool
{
public:
	static LineTool *getInstance(Point *mouse_point, QGraphicsScene *scene);

	void click(Point * clicked_point, bool existing_point = false) override;
	void resetTool() override;

private:
	LineTool(Point *mouse_point, QGraphicsScene *scene);
	static LineTool *instance;

	//working variables
	Point *clicked_points[2] = {nullptr, nullptr};

	//preview
	Line *line_preview;
	Point *line_preview_start_point;

};

#endif // LINETOOL_H
