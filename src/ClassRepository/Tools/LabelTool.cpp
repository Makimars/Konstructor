#include "LabelTool.h"

LabelTool *LabelTool::instance = nullptr;

LabelTool::LabelTool()
{

}

LabelTool *LabelTool::getInstance()
{
	if(LabelTool::instance == nullptr)
		LabelTool::instance = new LabelTool();

	return LabelTool::instance;
}

void LabelTool::click(Point *clicked_point, bool existing_point)
{

}

void LabelTool::resetTool()
{

}
