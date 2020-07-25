#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include "CircleTool.h"

class RectangleTool : public Plane::Tool
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

	/**
	 * p0  l0 p1
	 *
	 * l1	  l2
	 *
	 * p2  l3  p3
	 */
	Line *linesPreview[4];
	Point *pointsPreview[4];

	Point *mousePoint;

	int clickCounter;

	static RectangleTool *instance;

public slots:
	void mouseMoved(Point *mousePoint);
};

#endif // RECTANGLETOOL_H
