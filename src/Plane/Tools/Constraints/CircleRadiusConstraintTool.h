#ifndef CIRCLERADIUSCONSTRAINTTOOL_H
#define CIRCLERADIUSCONSTRAINTTOOL_H

#include "../Tool.h"

class CircleRadiusConstraintTool : public Tool
{
public:
	static CircleRadiusConstraintTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

private:
	CircleRadiusConstraintTool();
	static CircleRadiusConstraintTool *instance;
};

#endif // CIRCLERADIUSCONSTRAINTTOOL_H
