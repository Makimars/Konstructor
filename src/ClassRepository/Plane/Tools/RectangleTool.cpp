#include "RectangleTool.h"

RectangleTool *RectangleTool::instance = nullptr;

RectangleTool::RectangleTool() : Tool()
{
	clickCounter = 0;

	for(int i = 0; i < 4; i++)
	{
		pointsPreview[i] = objectFactory->makePoint();
		objectFactory->addToScene(pointsPreview[i]);
	}

	linesPreview[0] = objectFactory->makeLine(pointsPreview[0], pointsPreview[1]);
	linesPreview[1] = objectFactory->makeLine(pointsPreview[0], pointsPreview[2]);
	linesPreview[2] = objectFactory->makeLine(pointsPreview[1], pointsPreview[3]);
	linesPreview[3] = objectFactory->makeLine(pointsPreview[2], pointsPreview[3]);

	for(int i = 0; i < 4; i++)
		objectFactory->addToScene(linesPreview[i]);

}
RectangleTool *RectangleTool::getInstance()
{
	if(instance == nullptr)
		instance = new RectangleTool();
	return RectangleTool::instance;
}

void RectangleTool::click(DrawableObject *clickedObject, QPointF pos)
{
	Point *clickedPoint = dynamic_cast<Point*>(clickedObject);

	if(clickedPoint == nullptr)
		clickedPoint = objectFactory->makePoint(pos.x(), pos.y());

	clickCounter++;

	if(clickCounter == 1)
	{
		pointsPreview[0]->setLocation(pos);
		pointsPreview[1]->setX(pos.x());
		pointsPreview[2]->setY(pos.y());

		for(int i = 0; i < 4; i++)
		{
			pointsPreview[i]->setHidden(false);
			linesPreview[i]->setHidden(false);
		}
	}
	else if(clickCounter == 2)
	{
		for(int i = 0; i < 4; i++)
		{
			pointsPreview[i]->setHidden(true);
			linesPreview[i]->setHidden(true);
		}

		Point *newPoints[4];
		for(int i = 0; i < 4; i++)
		{
			newPoints[i] = objectFactory->copyPoint(pointsPreview[i]);
			objectFactory->addDrawable(newPoints[i]);
		}
		objectFactory->addDrawable(
					objectFactory->makeLine(newPoints[0], newPoints[1])
					);
		objectFactory->addDrawable(
					objectFactory->makeLine(newPoints[0], newPoints[2])
					);
		objectFactory->addDrawable(
					objectFactory->makeLine(newPoints[1], newPoints[3])
					);
		objectFactory->addDrawable(
					objectFactory->makeLine(newPoints[2], newPoints[3])
					);

		clickCounter = 0;
	}
}

void RectangleTool::resetTool()
{
	for(int i = 0; i < 4; i++)
	{
		pointsPreview[i]->setHidden(true);
		linesPreview[i]->setHidden(true);
	}
	clickCounter = 0;
}

void RectangleTool::mouseMoveEvent(QMouseEvent *event)
{
	if(clickCounter == 1)
	{
		pointsPreview[1]->setY(event->pos().y());
		pointsPreview[2]->setX(event->pos().x());
		pointsPreview[3]->setLocation(event->pos());
	}
}
