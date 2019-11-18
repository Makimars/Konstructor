#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include "CircleTool.h"

class RectangleTool : public Tool
{
public:
	static void initialise(Point *mouse_point,
						   QGraphicsScene *scene,
						   QBrush *default_brush,
						   QPen *default_pen
						   );
	static RectangleTool *getInstance();

	void click(DrawableObject *clicked_point, Point *mouse_point) override;
	void resetTool() override;

private:
	RectangleTool(Point *mouse_point, QGraphicsScene *scene);

	static RectangleTool *instance;
};

#endif // RECTANGLETOOL_H
