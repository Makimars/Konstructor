#ifndef DIMENSIONSTOOL_H
#define DIMENSIONSTOOL_H

#include "Tool.h"

class DimensionTool : public Tool
{
public:
	static DimensionTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;
	void resetTool() override;

public slots:
	void mouseMoveEvent(QPointF pos) override;

private:
	DimensionTool();
	static DimensionTool *instance;

	//working variables
	DrawableObject *clickedObjects[2] = {nullptr, nullptr};

	DrawableObject *dimensionPreview = nullptr;
};

#endif // DIMENSIONSTOOL_H
