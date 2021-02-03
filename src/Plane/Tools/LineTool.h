#ifndef LINETOOL_H
#define LINETOOL_H

#include "Tool.h"

class LineTool : public Tool
{
public:
	static LineTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

public slots:
	void mouseMoveEvent(QPointF pos) override;

private:
	LineTool();
	static LineTool *instance;

	//working variables
	Point *clickedPoints[2];

	//preview
	Line *linePreview;
	Point *previewPoints[2];

};

#endif // LINETOOL_H
