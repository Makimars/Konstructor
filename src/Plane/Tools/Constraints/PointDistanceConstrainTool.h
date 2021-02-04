#ifndef LINELENGTHCONSTRAINTOOL
#define LINELENGTHCONSTRAINTOOL

#include "../Tool.h"

class PointDistanceConstrainTool : public Tool
{
public:
	static PointDistanceConstrainTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

private:
	PointDistanceConstrainTool();
	static PointDistanceConstrainTool *instance;

	Point *firstPoint;
};

#endif // LINELENGTHCONSTRAINTOOL
