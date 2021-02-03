#ifndef LINELENGTHCONSTRAINTOOL
#define LINELENGTHCONSTRAINTOOL

#include "ArcTool.h"

class LineLengthConstrainTool : public Tool
{
public:
	static LineLengthConstrainTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

public slots:
	void mouseMoveEvent(QPointF pos) override;

private:
	LineLengthConstrainTool();
	static LineLengthConstrainTool *instance;

	Point *firstPoint;
};

#endif // LINELENGTHCONSTRAINTOOL
