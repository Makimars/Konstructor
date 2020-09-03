#include "DimensionTool.h"

DimensionTool *DimensionTool::instance = nullptr;

DimensionTool::DimensionTool(Point *mousePoint, QGraphicsScene *scene)
{
	this->objectFactory = Factory::getInstance();
}

void DimensionTool::initialize(Point *mousePoint,
							QGraphicsScene *scene,
							QBrush *defaultBrush,
							QPen *defaultPen)
{
	if(DimensionTool::instance == nullptr)
		DimensionTool::instance = new DimensionTool(mousePoint, scene);

	DimensionTool::instance->setCurrentPen(defaultPen);
	DimensionTool::instance->setCurrentBrush(defaultBrush);
}

DimensionTool *DimensionTool::getInstance()
{
	return DimensionTool::instance;
}

void DimensionTool::click(DrawableObject *clickedObject, Point *mousePoint)
{
	if(clickedObject != nullptr)
	{
		clickedObject->setHighlight(true);

		if(clickedObject->getId() <= 0) clickedObject = nullptr;
	}

	this->clickedObjects[1] = this->clickedObjects[0];
	this->clickedObjects[0] = clickedObject;

	if(this->clickedObjects[0] != nullptr & this->clickedObjects[1] != nullptr)
	{
		// two not null drawables

		int object0Type = this->clickedObjects[0]->getType();
		int object1Type = this->clickedObjects[1]->getType();

		if(object0Type == Global::Line & object1Type == Global::Line)
		{
			// two lines

			Line *lines[] = {
				dynamic_cast<Line*>(this->clickedObjects[0]),
				dynamic_cast<Line*>(this->clickedObjects[1])
			};


			if(lines[0]->getStartPoint() == lines[1]->getStartPoint()		|
					lines[0]->getStartPoint() == lines[1]->getEndPoint()	|
					lines[0]->getEndPoint() == lines[1]->getStartPoint()	|
					lines[0]->getEndPoint() == lines[1]->getEndPoint()		)
			{
				// one common point
				Point *commonPoint;
				if(lines[0]->getStartPoint() == lines[1]->getStartPoint()	|
					lines[0]->getStartPoint() == lines[1]->getEndPoint()	)
					commonPoint = lines[0]->getStartPoint();
				else
					commonPoint = lines[0]->getEndPoint();

				//two lines angle
				this->objectFactory->addDrawable(
							this->objectFactory->makeLinesAngleDimension(lines)
							);
			}
			else
			{
				// no common point
				//two lines distance
			}
		}
		else if(object0Type == Global::Circle & object0Type == Global::Circle)
		{
			//two circles
			Circle *circles[] = {
				dynamic_cast<Circle*>(this->clickedObjects[0]),
				dynamic_cast<Circle*>(this->clickedObjects[1])
			};

			if(circles[0]->getCenterPoint() == circles[1]->getCenterPoint())
			{
				//common center

				this->objectFactory->addDrawable(
							this->objectFactory->makeCirclesRadiusDifferenceDimension(circles)
							);
			}
		}
		else
		{

		}

		this->clickedObjects[0]->setHighlight(false);
		this->clickedObjects[1]->setHighlight(false);
	}
	else if(this->clickedObjects[0] == nullptr & this->clickedObjects[1] != nullptr)
	{
		// second drawable is null

		if(this->clickedObjects[1]->getType() == Global::Line)
		{
			//line and place
qDebug() << "line and location";
			Line *line = dynamic_cast<Line*>(this->clickedObjects[1]);

			double distanceFromLine = -line->signedDistanceFrom(mousePoint->getLocation());

			this->objectFactory->addDrawable(
				this->objectFactory->makeLineLengthDimension(
							line,
							line->getLength(),
							distanceFromLine
							)
				);
		}
		else if(this->clickedObjects[1]->getType() == Global::Circle)
		{
			Circle *circle = dynamic_cast<Circle*>(this->clickedObjects[1]);

			this->objectFactory->addDrawable(
						this->objectFactory->makeCircleRadiusDimension(circle)
						);
		}

		this->clickedObjects[1]->setHighlight(false);
	}
	else if (this->clickedObjects[0] == nullptr)
	{

	}

}

void DimensionTool::resetTool()
{
	if(this->clickedObjects[0] != nullptr)
	{
		this->clickedObjects[0]->setHighlight(false);
		this->clickedObjects[0] = nullptr;
	}

	if(this->clickedObjects[1] != nullptr)
	{
		this->clickedObjects[1]->setHighlight(false);
		this->clickedObjects[1] = nullptr;
	}
}
