#include "LinesAngleConstraintTool.h"

LinesAngleConstraintTool *LinesAngleConstraintTool::instance = nullptr;

LinesAngleConstraintTool *LinesAngleConstraintTool::getInstance()
{
	if(LinesAngleConstraintTool::instance == nullptr)
		LinesAngleConstraintTool::instance = new LinesAngleConstraintTool();
	return LinesAngleConstraintTool::instance;
}

LinesAngleConstraintTool::LinesAngleConstraintTool()
{
	toolTips.append(tr("Select the first line."));
	toolTips.append(tr("Select the second line."));
}

void LinesAngleConstraintTool::click(DrawableObject *clickedObject, QPointF pos)
{

}

void LinesAngleConstraintTool::resetTool()
{
	clickCounter = 0;
	firstLine = nullptr;
}

