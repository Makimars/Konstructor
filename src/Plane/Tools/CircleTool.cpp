#include "CircleTool.h"

CircleTool *CircleTool::instance = nullptr;

CircleTool::CircleTool() : Tool()
{
	this->circlePreviewCentre = this->planeFactory->makePoint();
	this->circlePreview = this->planeFactory
			->makeCircle(this->circlePreviewCentre);

	this->circlePreview->setHidden(true);
	this->circlePreviewCentre->setHidden(true);

	planeFactory->addToScene(circlePreview);
	planeFactory->addToScene(circlePreviewCentre);

	toolTips.append(tr("Select center for a circle"));
	toolTips.append(tr("Select circle's radius"));
}

CircleTool *CircleTool::getInstance()
{
	if(CircleTool::instance == nullptr)
		CircleTool::instance = new CircleTool();
	return CircleTool::instance;
}

void CircleTool::click(DrawableObject *clickedObject, QPointF pos)
{
	clickCounter++;

	if(clickCounter == 1)
	{
		//returns nullptr if it fails
		this->clickedPoint = dynamic_cast<Point*>(clickedObject);

		this->circlePreview->setHidden(false);
		this->circlePreviewCentre->setHidden(false);

		circlePreviewCentre->setLocation(pos);
	}
	else if (clickCounter == 2)
	{
		Point *centrePoint;
		if(clickedPoint != nullptr)
		{
			centrePoint = clickedPoint;
		}
		else
		{
			centrePoint = planeFactory->copyPoint(circlePreviewCentre);
			planeFactory->addDrawable(centrePoint);
		}

		planeFactory->addDrawable(
					planeFactory->makeCircle(centrePoint, circlePreview->getRadius())
					);

		resetTool();
	}
}

void CircleTool::resetTool()
{
	clickCounter = 0;
	this->circlePreview->setHidden(true);
	this->circlePreviewCentre->setHidden(true);
}

void CircleTool::mouseMoveEvent(QPointF pos)
{
	if(clickCounter == 1)
	{
		circlePreview->setRadius(circlePreviewCentre->distanceFrom(pos));
	}
}
