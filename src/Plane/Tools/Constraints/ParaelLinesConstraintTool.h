#ifndef PARAELLINESCONSTRAINTTOOL_H
#define PARAELLINESCONSTRAINTTOOL_H

#include "../Tool.h"

class ParaelLinesConstraintTool : public Tool
{
public:
	static ParaelLinesConstraintTool *getInstance();

	void click(DrawableObject * clickedObject, QPointF pos) override;
	void resetTool() override;

private:
	ParaelLinesConstraintTool();
	static ParaelLinesConstraintTool *instance;

	Line *clickedLine;
};

#endif // PARAELLINESCONSTRAINTTOOL_H
