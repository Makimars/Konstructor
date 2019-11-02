#ifndef LABELTOOL_H
#define LABELTOOL_H

#include "RectangleTool.h"

class LabelTool : public Tool
{
public:
	static LabelTool *getInstance();

	void click(Point *clicked_point, bool existing_point = false) override;
	void resetTool() override;

private:
	LabelTool();

	static LabelTool *instance;
};

#endif // LABELTOOL_H
