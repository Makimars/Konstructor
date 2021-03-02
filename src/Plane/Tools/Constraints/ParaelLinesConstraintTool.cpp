#include "ParaelLinesConstraintTool.h"

ParaelLinesConstraintTool *ParaelLinesConstraintTool::instance = nullptr;

ParaelLinesConstraintTool *ParaelLinesConstraintTool::getInstance()
{
	if(ParaelLinesConstraintTool::instance == nullptr)
		ParaelLinesConstraintTool::instance = new ParaelLinesConstraintTool();
	return ParaelLinesConstraintTool::instance;
}

ParaelLinesConstraintTool::ParaelLinesConstraintTool() : Tool()
{
	toolTips.append(tr("Select the line determinig the vector."));
	toolTips.append(tr("Select the line to be adjusted."));
}

void ParaelLinesConstraintTool::click(DrawableObject *clickedObject, QPointF pos)
{
	if(clickedObject != nullptr)
	{
		if(clickedObject->getType() == Global::Line)
		{
			clickCounter++;
			Line *currentLine = dynamic_cast<Line*>(clickedObject);

			if(clickCounter == 1)
			{
				clickedLine = currentLine;
			}
			else if(clickCounter == 2)
			{
				if((clickedLine->getStartPoint()->isLocked() & clickedLine->getEndPoint()->isLocked())
					|| (currentLine->getStartPoint()->isLocked() & currentLine->getEndPoint()->isLocked()))
				{
					resetTool();
					return;
				}

				objectFactory->addDrawable(
							objectFactory->makeParaelLinesConstraint(clickedLine, currentLine)
							);
				resetTool();
			}
		}
	}
}

void ParaelLinesConstraintTool::resetTool()
{
	clickCounter = 0;
	clickedLine = nullptr;
}

