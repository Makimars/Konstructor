#ifndef ARCTOOL_H
#define ARCTOOL_H

#include "DimensionTool.h"

class ArcTool : public Tool
{
public:
	static ArcTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

public slots:
	void mouseMoveEvent(QPointF pos) override;

private:
	ArcTool();
	static ArcTool *instance;

	//working variables
	Point *clickedPoints[3] = {nullptr, nullptr, nullptr};
	Arc *preview;
	Point *previewPoints[3];
};

#endif // ARCTOOL_H
