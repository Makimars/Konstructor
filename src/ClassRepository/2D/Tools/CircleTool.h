#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include "LineTool.h"

class CircleTool : public Plane::Tool
{
public:
	static void initialize(Point *mousePoint,
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
