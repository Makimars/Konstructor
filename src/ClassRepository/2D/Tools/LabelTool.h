#ifndef LABELTOOL_H
#define LABELTOOL_H

#include "RectangleTool.h"

class LabelTool : public Tool
{
public:
	static void initialize(Point *mousePoint,
						   QGraphicsScene *scene,
						   QBrush *defaultBrush,
						   QPen *defaultPen
						   );
	static LabelTool *getInstance();

	void click(DrawableObject *clickedPoint, Point *mousePoint) override;
	void resetTool() override;

private:
	LabelTool(Point *mousePoint, QGraphicsScene *scene);

	static LabelTool *instance;
};

#endif // LABELTOOL_H
