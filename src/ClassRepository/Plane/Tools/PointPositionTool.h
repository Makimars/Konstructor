#ifndef ABSOLUTEPOINTPOSITION_H
#define ABSOLUTEPOINTPOSITION_H

#include "ArcTool.h"
#include "../../../PointPositionDialog.h"

class PointPositionTool : public Tool
{
public:
	static PointPositionTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

public slots:
	void mouseMoveEvent(QPointF pos) override;

private:
	PointPositionTool();
	~PointPositionTool();
	static PointPositionTool *instance;

	PointPositionDialog *dialog;
};

#endif // ABSOLUTEPOINTPOSITION_H
