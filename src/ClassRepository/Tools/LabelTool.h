#ifndef LABELTOOL_H
#define LABELTOOL_H

#include "RectangleTool.h"

class LabelTool : public Tool
{
public:
	static void initialise(Point *mouse_point,
						   QGraphicsScene *scene,
						   QBrush *default_brush,
						   QPen *default_pen
						   );
	static LabelTool *getInstance();

	void click(DrawableObject *clicked_point, Point *mouse_point) override;
	void resetTool() override;

private:
	LabelTool(Point *mouse_point, QGraphicsScene *scene);

	static LabelTool *instance;
};

#endif // LABELTOOL_H
