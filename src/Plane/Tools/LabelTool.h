#ifndef LABELTOOL_H
#define LABELTOOL_H

#include "Tool.h"

class LabelTool : public Tool
{
public:
	static LabelTool *getInstance();

	void click(DrawableObject *clickedObject, QPointF pos) override;

private:
	LabelTool();

	static LabelTool *instance;
};

#endif // LABELTOOL_H
