#include "PointDistanceConstrainTool.h"

PointDistanceConstrainTool *PointDistanceConstrainTool::instance = nullptr;

PointDistanceConstrainTool::PointDistanceConstrainTool() : Tool()
{
	toolTips.append(tr("Select first point (origin point), this point will be fixed"));
	toolTips.append(tr("Select second point which will be driven by the distance"));
}

PointDistanceConstrainTool *PointDistanceConstrainTool::getInstance()
{
	if(PointDistanceConstrainTool::instance == nullptr)
		PointDistanceConstrainTool::instance = new PointDistanceConstrainTool();
	return PointDistanceConstrainTool::instance;
}

void PointDistanceConstrainTool::click(DrawableObject *clickedObject, QPointF pos)
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
				if(clickedObject->isLocked() | clickedObject->isConstrained())
				{
					clickCounter = 1;
					return;
				}

				objectFactory->addDrawable(
					objectFactory->makePointDistanceConstraint(firstPoint, dynamic_cast<Point*>(clickedObject))
				);
				resetTool();
			}
		}
	}
}

void PointDistanceConstrainTool::resetTool()
{
	clickCounter = 0;
	firstPoint = nullptr;
}
