#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include "CircleTool.h"

class RectangleTool : public Tool
{
public:
	static void initialize(Point *mousePoint,
						   QGraphicsScene *scene,
						   QBrush *defaultBrush,
						   QPen *defaultPen
						   );
	static RectangleTool *getInstance();

	void click(DrawableObject *clickedPoint, Point *mousePoint) override;
	void resetTool() override;

private:
	RectangleTool(Point *mousePoint, QGraphicsScene *scene);

	static RectangleTool *instance;
};

#endif // RECTANGLETOOL_H
