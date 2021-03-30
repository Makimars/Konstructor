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
	if(clickedObject->getType() == Global::Line)
	{
		clickCounter++;
		if(clickCounter == 1)
		{
			firstLine = dynamic_cast<Line*>(clickedObject);
		}
		else if(clickCounter == 2)
		{
			Line *secLine = dynamic_cast<Line*>(clickedObject);
			bool valid = false;
			valid = valid | (firstLine->getStartPoint() == secLine->getStartPoint());
			valid = valid | (firstLine->getStartPoint() == secLine->getEndPoint());
			valid = valid | (firstLine->getEndPoint() == secLine->getStartPoint());
			valid = valid | (firstLine->getEndPoint() == secLine->getEndPoint());

			if(valid)
			{
				planeFactory->addDrawable(
							planeFactory->makeLinesAngleConstraint(firstLine, secLine)
						);
			}
			else
			{
				emit showStatusBarMessage("Lines does not have a common point.");
			}
			resetTool();
		}
	}
}

void LinesAngleConstraintTool::resetTool()
{
	clickCounter = 0;
	firstLine = nullptr;
}

