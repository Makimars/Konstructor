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
	if(this->clickedObjects[1] != nullptr)
		this->objectFactory->tryDeleteDrawable(this->clickedObjects[1]);
	this->clickedObjects[1] = this->clickedObjects[0];
	this->clickedObjects[0] = clickedObject;

	if(this->clickedObjects[0] != nullptr & this->clickedObjects[1] != nullptr)
	{
		QString object0Type = this->clickedObjects[0]->getType();
		QString object1Type = this->clickedObjects[1]->getType();

		if(object0Type == TYPE_LINE & object1Type == TYPE_LINE)
		{// two lines

			Line *lines[] = {
				dynamic_cast<Line*>(this->clickedObjects[0]),
				dynamic_cast<Line*>(this->clickedObjects[1])
			};

			// one common point
			if((lines[0]->getStartPoint() == lines[1]->getStartPoint()) |
					(lines[0]->getStartPoint() == lines[1]->getEndPoint()))
			{
				//two lines angle

			}
			else // no common point
			{
				//two lines distance
			}
		}
		else
		{

		}

		this->clickedObjects[0]->setHighlight(false);
		this->clickedObjects[1]->setHighlight(false);
	}
	else if(this->clickedObjects[0] != nullptr & this->clickedObjects[1] == nullptr)
	{
		if(clickedObject->getType() == TYPE_LINE)
		{
			Line *line = dynamic_cast<Line*>(clickedObject);
			this->objectFactory->addDrawable(
				this->objectFactory->makeDimension(line, line->getLength())
											);
			clickedObject->setHighlight(false);
			this->clickedObjects[0] = nullptr;
		}
	}
	else if (this->clickedObjects[0] == nullptr)
	{
		if(this->clickedObjects[1] != nullptr)
			this->objectFactory->tryDeleteDrawable(this->clickedObjects[1]);
	}

}

void DimensionTool::resetTool()
{
	this->objectFactory->tryDeleteDrawable(this->clickedObjects[0]);
	this->clickedObjects[0] = nullptr;

	this->objectFactory->tryDeleteDrawable(this->clickedObjects[1]);
	this->clickedObjects[1] = nullptr;
}
