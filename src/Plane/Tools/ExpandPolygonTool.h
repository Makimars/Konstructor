#ifndef EXPANDPOLYGONTOOL_H
#define EXPANDPOLYGONTOOL_H

#include "Tool.h"

class ExpandPolygonTool : public Tool
{
public:
	static ExpandPolygonTool *getInstance();

	void click(DrawableObject * clickedObject, QPointF pos) override;
	void resetTool() override;

public slots:
	void mouseMoveEvent(QPointF pos) override;

private:
	ExpandPolygonTool();
	static ExpandPolygonTool *instance;

	Point *previewCircleCenter;
	Circle *previewCircle;
	Circle *clickedCircle;
};

#endif // EXPANDPOLYGONTOOL_H
