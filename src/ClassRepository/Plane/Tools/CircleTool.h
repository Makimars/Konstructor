#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include "LineTool.h"

class CircleTool : public Tool
{
public:
	static CircleTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

public slots:
	void mouseMoveEvent(QPointF pos) override;

private:
	CircleTool();
	static CircleTool *instance;

	//working variables
	Point *clickedPoint;

	//preview
	Circle *circlePreview;
	Point *circlePreviewCentre;
};

#endif // CIRCLETOOL_H
