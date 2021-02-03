#include "LineLengthConstrainTool.h"

LineLengthConstrainTool *LineLengthConstrainTool::instance = nullptr;

LineLengthConstrainTool::LineLengthConstrainTool() : Tool(){}

LineLengthConstrainTool *LineLengthConstrainTool::getInstance()
{
	if(LineLengthConstrainTool::instance == nullptr)
		LineLengthConstrainTool::instance = new LineLengthConstrainTool();
	return LineLengthConstrainTool::instance;
}

void LineLengthConstrainTool::click(DrawableObject *clickedObject, QPointF pos)
{
	if(clickedObject != nullptr)
	{
		clickedObject->setHighlight(true);

		if(clickedObject->getType() == Global::Types::Point)
		{
			clickCounter++;

			if(clickCounter == 1)
			{
				firstPoint = dynamic_cast<Point*>(clickedObject);
			}
			else if(clickCounter == 2)
			{
				objectFactory->addDrawable(
					objectFactory->makeLengthConstraint(firstPoint, dynamic_cast<Point*>(clickedObject))
				);
				resetTool();
			}
		}
	}
}

void LineLengthConstrainTool::resetTool()
{
	clickCounter = 0;
	firstPoint = nullptr;
}

void LineLengthConstrainTool::mouseMoveEvent(QPointF pos){}
