#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include "CircleTool.h"

class RectangleTool : public Tool
{
public:
	static RectangleTool *getInstance();

	void click(Point *clicked_point, bool existing_point = false) override;
	void resetTool() override;

private:
	RectangleTool();

	static RectangleTool *instance;
};

#endif // RECTANGLETOOL_H
