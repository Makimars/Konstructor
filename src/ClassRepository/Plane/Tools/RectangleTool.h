#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include "CircleTool.h"

class RectangleTool : public Tool
{
public:
	static RectangleTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

public slots:
	void mouseMoveEvent(QMouseEvent *event);

private:
	RectangleTool();
	static RectangleTool *instance;

	/**
	 * p0  l0 p1
	 *
	 * l1	  l2
	 *
	 * p2  l3  p3
	 */
	Line *linesPreview[4];
	Point *pointsPreview[4];
	int clickCounter;
};

#endif // RECTANGLETOOL_H
