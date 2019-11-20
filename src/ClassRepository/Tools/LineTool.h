#ifndef LINETOOL_H
#define LINETOOL_H

#include "Tool.h"

class LineTool : public Tool
{
public:
	static void initialise(Point *mousePoint,
						   QGraphicsScene *scene,
						   QBrush *defaultBrush,
						   QPen *defaultPen
						   );
	static LineTool *getInstance();

	void click(DrawableObject *clickedObject, Point *mousePoint) override;
	void resetTool() override;

private:
	LineTool(Point *mousePoint, QGraphicsScene *scene);
	static LineTool *instance;

	//working variables
	Point *clickedPoints[2] = {nullptr, nullptr};

	//preview
	Line *linePreview;
	Point *linePreviewStartPoint;

};

#endif // LINETOOL_H
