#include "RectangleTool.h"

RectangleTool *RectangleTool::instance = nullptr;

RectangleTool::RectangleTool() : Tool()
{
	for(int i = 0; i < 4; i++)
	{
		pointsPreview[i] = planeFactory->makePoint();
		planeFactory->addToScene(pointsPreview[i]);
	}

	linesPreview[0] = planeFactory->makeLine(pointsPreview[0], pointsPreview[1]);
	linesPreview[1] = planeFactory->makeLine(pointsPreview[0], pointsPreview[2]);
	linesPreview[2] = planeFactory->makeLine(pointsPreview[1], pointsPreview[3]);
	linesPreview[3] = planeFactory->makeLine(pointsPreview[2], pointsPreview[3]);

	for(int i = 0; i < 4; i++)
		planeFactory->addToScene(linesPreview[i]);

	for(int i = 0; i < 4; i++)
	{
		pointsPreview[i]->setHidden(true);
		linesPreview[i]->setHidden(true);
	}

	toolTips.append(tr("Select edge of the rectangle."));
	toolTips.append(tr("Select size of the rectangle."));
}
RectangleTool *RectangleTool::getInstance()
{
	if(instance == nullptr)
		instance = new RectangleTool();
	return RectangleTool::instance;
}

void RectangleTool::click(DrawableObject *clickedObject, QPointF pos)
{
	clickCounter++;

	if(clickCounter == 1)
	{
		//returns nullptr if it fails
		clickedPoints[0] = dynamic_cast<Point*>(clickedObject);

		if(clickedPoints[0] == nullptr)
		{
			pointsPreview[0]->setLocation(pos);
			pointsPreview[1]->setX(pos.x());
			pointsPreview[2]->setY(pos.y());
		}
		else
		{
			pointsPreview[0]->setLocation(clickedPoints[0]->getLocation());
			pointsPreview[1]->setX(clickedPoints[0]->getX());
			pointsPreview[2]->setY(clickedPoints[0]->getY());
		}

		for(int i = 0; i < 4; i++)
		{
			pointsPreview[i]->setHidden(false);
			linesPreview[i]->setHidden(false);
		}
	}
	else if(clickCounter == 2)
	{
		//returns nullptr if it fails
		clickedPoints[1] = dynamic_cast<Point*>(clickedObject);

		//create points (edge points might already be created)
		Point *newPoints[4];

		if(clickedPoints[0] != nullptr)
			newPoints[0] = clickedPoints[0];
		else
			newPoints[0] = planeFactory->copyPoint(pointsPreview[0]);

		newPoints[1] = planeFactory->copyPoint(pointsPreview[1]);
		newPoints[2] = planeFactory->copyPoint(pointsPreview[2]);

		if(clickedPoints[1] != nullptr)
		{
			newPoints[3] = clickedPoints[1];

			pointsPreview[1]->setY(clickedPoints[1]->getY());
			pointsPreview[2]->setX(clickedPoints[1]->getX());
		}
		else
			newPoints[3] = planeFactory->copyPoint(pointsPreview[3]);

		//add points to drawing
		for(int i = 0; i < 4; i++)
			planeFactory->addDrawable(newPoints[i]);

		//create lines
		planeFactory->addDrawable(
					planeFactory->makeLine(newPoints[0], newPoints[1])
					);
		planeFactory->addDrawable(
					planeFactory->makeLine(newPoints[0], newPoints[2])
					);
		planeFactory->addDrawable(
					planeFactory->makeLine(newPoints[1], newPoints[3])
					);
		planeFactory->addDrawable(
					planeFactory->makeLine(newPoints[2], newPoints[3])
					);


		resetTool();
	}
}

void RectangleTool::resetTool()
{
	for(int i = 0; i < 4; i++)
	{
		pointsPreview[i]->setHidden(true);
		linesPreview[i]->setHidden(true);
	}
	clickedPoints[0] = nullptr;
	clickedPoints[1] = nullptr;
	clickCounter = 0;
}

void RectangleTool::mouseMoveEvent(QPointF pos)
{
	if(clickCounter == 1)
	{
		pointsPreview[1]->setY(pos.y());
		pointsPreview[2]->setX(pos.x());
		pointsPreview[3]->setLocation(pos);
	}
}
