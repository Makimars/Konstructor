#ifndef ARCTOOL_H
#define ARCTOOL_H

#include "DimensionTool.h"

class ArcTool : public Tool
{
public:
	static DimensionTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

private:
	ArcTool();
	static ArcTool *instance;

	//working variables
	Point *clickedPoints[2] = {nullptr, nullptr};
	Arc *preview;
	Point *previewPoints[3];
};

#endif // ARCTOOL_H
