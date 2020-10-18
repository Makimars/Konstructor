#ifndef LINETOOL_H
#define LINETOOL_H

#include "Tool.h"

class LineTool : public Tool
{
public:
	static LineTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

private:
	LineTool();
	static LineTool *instance;

	//working variables
	Point *clickedPoints[2] = {nullptr, nullptr};

	//preview
	Line *linePreview;
	Point *linePreviewStartPoint;

};

#endif // LINETOOL_H
