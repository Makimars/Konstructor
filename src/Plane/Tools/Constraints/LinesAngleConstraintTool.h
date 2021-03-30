#ifndef LINESANGLECONSTRAINTTOOL_H
#define LINESANGLECONSTRAINTTOOL_H

#include "../Tool.h"

class LinesAngleConstraintTool : public Tool
{
	Q_OBJECT
public:
	static LinesAngleConstraintTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

signals:
	void showStatusBarMessage(const QString &message, int timeout = 0);

private:
	LinesAngleConstraintTool();
	static LinesAngleConstraintTool *instance;

	Line *firstLine;
};

#endif // LINESANGLECONSTRAINTTOOL_H
