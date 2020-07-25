#include "RectangleTool.h"

RectangleTool *RectangleTool::instance = nullptr;

RectangleTool::RectangleTool(Point *mousePoint, QGraphicsScene *scene)
{
	this->objectFactory = Factory::getInstance();
	this->mousePoint = mousePoint;
	clickCounter = 0;
}

void RectangleTool::initialize(Point *mousePoint,
							QGraphicsScene *scene,
							QBrush *defaultBrush,
							QPen *defaultPen)
{
	if(RectangleTool::instance == nullptr)
		RectangleTool::instance = new RectangleTool(mousePoint, scene);

	RectangleTool::instance->setCurrentPen(defaultPen);
	RectangleTool::instance->setCurrentBrush(defaultBrush);
}

RectangleTool *RectangleTool::getInstance()
{
	return RectangleTool::instance;
}

void RectangleTool::click(DrawableObject *clickedObject, Point *mousePoint)
{
	if(clickedObject == nullptr)
		clickedObject = mousePoint->clone();

	if(clickedObject->getType() != Global::Point)
		clickedObject = mousePoint->clone();

	if(clickedObject->getType() == Global::Point)
	{
		Point *clickedPoint = dynamic_cast<Point*>(clickedObject);
		clickCounter++;

		if(clickCounter == 1)
		{
			pointsPreview[0] = clickedPoint;
			pointsPreview[1] = new Point(clickedPoint->getX(), 0);
			pointsPreview[2] = new Point(0, clickedPoint->getY());
			pointsPreview[3] = new Point();

			linesPreview[0] = new Line(pointsPreview[0], pointsPreview[1]);
			linesPreview[1] = new Line(pointsPreview[0], pointsPreview[2]);
			linesPreview[2] = new Line(pointsPreview[1], pointsPreview[3]);
			linesPreview[3] = new Line(pointsPreview[2], pointsPreview[3]);

			for(int i = 0; i < 4; i++)
				objectFactory->addToScene(linesPreview[i]);

			for(int i = 0; i < 4; i++)
				objectFactory->addToScene(pointsPreview[i]);
		}
		else if(clickCounter == 2)
		{
			for(int i = 0; i < 4; i++)
				objectFactory->addDrawable(linesPreview[i]);

			for(int i = 0; i < 4; i++)
				objectFactory->addDrawable(pointsPreview[i]);

			for(int i = 0; i < 4; i++)
			{
				linesPreview[i] = nullptr;
				pointsPreview[i] = nullptr;
			}
			clickCounter = 0;
		}
	}
}

void RectangleTool::resetTool()
{
	for(int i = 0; i < 4; i++)
	{
		objectFactory->tryDeleteDrawable(linesPreview[i]);
		linesPreview[i] = nullptr;
		objectFactory->tryDeleteDrawable(pointsPreview[i]);
		pointsPreview[i] = nullptr;
	}
	clickCounter = 0;
}

void RectangleTool::mouseMoved(Point *mousePoint)
{
	if(clickCounter == 1)
	{
		pointsPreview[1]->setY(mousePoint->getY());
		pointsPreview[2]->setX(mousePoint->getX());
		pointsPreview[3]->setLocation(mousePoint->getLocation());
	}
}
