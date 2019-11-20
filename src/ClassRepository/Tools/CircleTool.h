#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include "LineTool.h"

class CircleTool : public Tool
{
public:
	static void initialise(Point *mousePoint,
						   QGraphicsScene *scene,
						   QBrush *defaultBrush,
						   QPen *defaultPen
						   );
	static CircleTool *getInstance();

	void click(DrawableObject *clickedObject, Point *mousePoint) override;
	void resetTool() override;

private:
	CircleTool(Point *mousePoint, QGraphicsScene *scene);
	static CircleTool *instance;

	//working variables
	Point *previousClickedPoint;

	//preview
	Circle *circlePreview;
	Point *circlePreviewCentre;
};

#endif // CIRCLETOOL_H
