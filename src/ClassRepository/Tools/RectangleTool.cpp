#include "RectangleTool.h"

RectangleTool *RectangleTool::instance = nullptr;

RectangleTool::RectangleTool()
{

}

RectangleTool *RectangleTool::getInstance()
{
	if(RectangleTool::instance == nullptr)
		RectangleTool::instance = new RectangleTool();

	return RectangleTool::instance;
}

void RectangleTool::click(Point *clicked_point, bool existing_point)
{

}

void RectangleTool::resetTool()
{

}
