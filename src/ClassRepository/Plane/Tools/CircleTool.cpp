#include "CircleTool.h"

CircleTool *CircleTool::instance = nullptr;

CircleTool::CircleTool() : Tool()
{
	this->objectFactory = Factory::getInstance();

	this->circlePreviewCentre = this->objectFactory->makePoint();
	this->circlePreview = this->objectFactory
			->makeCircle(this->circlePreviewCentre);

	this->circlePreview->setHidden(true);
	this->circlePreviewCentre->setHidden(true);

	objectFactory->addToScene(circlePreview);
	objectFactory->addToScene(circlePreviewCentre);
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
			centrePoint = objectFactory->copyPoint(circlePreviewCentre);
			objectFactory->addDrawable(centrePoint);
		}

		objectFactory->addDrawable(
					objectFactory->makeCircle(centrePoint, circlePreview->getRadius())
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
