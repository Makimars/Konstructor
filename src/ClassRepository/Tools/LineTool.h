#ifndef LINETOOL_H
#define LINETOOL_H

#include "Tool.h"

class LineTool : public Tool
{
public:
	static void initialise(Point *mouse_point,
						   QGraphicsScene *scene,
						   QBrush *default_brush,
						   QPen *default_pen
						   );
	static LineTool *getInstance();

	void click(DrawableObject *clicked_object, Point *mouse_point) override;
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
