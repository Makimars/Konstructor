#include "DimensionTool.h"

DimensionTool *DimensionTool::instance = nullptr;

DimensionTool::DimensionTool(Point *mousePoint, QGraphicsScene *scene)
{
	this->objectFactory = DrawablesFactory::getInstance();
}

void DimensionTool::initialise(Point *mousePoint,
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
		clickedObject->setHighlight(true);
	this->clickedObjects[1] = this->clickedObjects[0];
	this->clickedObjects[0] = clickedObject;

	if(this->clickedObjects[0] != nullptr & this->clickedObjects[1] != nullptr)
	{
		// two not null drawables

		QString object0Type = this->clickedObjects[0]->getType();
		QString object1Type = this->clickedObjects[1]->getType();

		if(object0Type == TYPE_LINE & object1Type == TYPE_LINE)
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
							this->objectFactory->makeLinesAngleDimension(
								lines,
								Settings::defaultAngleDimensionTextDistance
								)
							);
			}
			else
			{
				// no common point
				//two lines distance
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

		if(this->clickedObjects[1]->getType() == TYPE_LINE)
		{
			//line and place

			Line *line = dynamic_cast<Line*>(this->clickedObjects[1]);

			double distanceFromLine = -line->signedDistanceFrom(mousePoint->getLocation());

			this->objectFactory->addDrawable(
				this->objectFactory->makeLineLengthDimension(
							line,
							line->getLength(),
							distanceFromLine
							)
				);
			this->clickedObjects[1]->setHighlight(false);
			this->clickedObjects[0] = nullptr;
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
