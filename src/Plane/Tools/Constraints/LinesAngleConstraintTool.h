#ifndef LINESANGLECONSTRAINTTOOL_H
#define LINESANGLECONSTRAINTTOOL_H

#include "../Tool.h"

class LinesAngleConstraintTool : public Tool
{
public:
	static LinesAngleConstraintTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

private:
	LinesAngleConstraintTool();
	static LinesAngleConstraintTool *instance;

	Line *firstLine;
};

#endif // LINESANGLECONSTRAINTTOOL_H
