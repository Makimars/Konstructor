#ifndef LINECENTERTOOL_H
#define LINECENTERTOOL_H

#include "Tool.h"

class LineCenterTool : public Tool
{
public:
	static LineCenterTool *getInstance();

	void click(DrawableObject * clickedObject, QPointF pos) override;

private:
	LineCenterTool();
	static LineCenterTool *instance;
};

#endif // LINECENTERTOOL_H
