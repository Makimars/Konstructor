#include "DimensionTool.h"

DimensionTool *DimensionTool::instance = nullptr;

DimensionTool::DimensionTool(Point *mousePoint, QGraphicsScene *scene)
{

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
	this->clickedObjects[1] = this->clickedObjects[0];
	this->clickedObjects[0] = clickedObject;

	if(this->clickedObjects[1] != nullptr)
	{
		QString object0Type = this->clickedObjects[0]->getType();
		QString object1Type = this->clickedObjects[1]->getType();

		if(object0Type == TYPE_LINE & object1Type == TYPE_LINE)
		{
			// two lines
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
		else if((object0Type == TYPE_POINT | object1Type == TYPE_POINT) &
				(object0Type == TYPE_LINE  | object1Type == TYPE_LINE))
		{
			//one line and one point



		}

	}


}

void DimensionTool::resetTool()
{
	this->objectFactory->tryDeleteDrawable(this->clickedObjects[0]);
	this->clickedObjects[0] = nullptr;

	this->objectFactory->tryDeleteDrawable(this->clickedObjects[1]);
	this->clickedObjects[1] = nullptr;
}
