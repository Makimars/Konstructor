#include "LineTool.h"

LineTool *LineTool::instance = nullptr;

LineTool::LineTool() : Tool()
{
	clickCounter = 0;

	//line preview
	this->previewPoints[0] = this->objectFactory->makePoint();
	this->previewPoints[1] = this->objectFactory->makePoint();
	this->linePreview = this->objectFactory->makeLine(previewPoints[0], previewPoints[1]);

	previewPoints[0]->setHidden(true);
	previewPoints[1]->setHidden(true);
	linePreview->setHidden(true);

	objectFactory->addToScene(previewPoints[0]);
	objectFactory->addToScene(previewPoints[1]);
	objectFactory->addToScene(linePreview);

	toolTips.append("Select frist point");
	toolTips.append("Select second point of a line");
}

LineTool *LineTool::getInstance()
{
	if(LineTool::instance == nullptr)
		LineTool::instance = new LineTool();
	return LineTool::instance;
}

void LineTool::click(DrawableObject *clickedObject, QPointF pos)
{
	clickCounter++;

	if(clickCounter == 1)
	{
		//returns nullptr if it fails
		clickedPoints[0] = dynamic_cast<Point*>(clickedObject);

		previewPoints[0]->setHidden(false);
		previewPoints[1]->setHidden(false);
		linePreview->setHidden(false);

		previewPoints[0]->setLocation(pos);
		if(clickedPoints[0] != nullptr)
			previewPoints[0]->setLocation(clickedPoints[0]->getLocation());
	}
	else if(clickCounter == 2)
	{
		//returns nullptr if it fails
		clickedPoints[1] = dynamic_cast<Point*>(clickedObject);

		Point *points[2];
		for(int i = 0; i < 2; i++)
		{
			if(clickedPoints[i] != nullptr)
			{
				points[i] = clickedPoints[i];
			}
			else
			{
				points[i] = objectFactory->copyPoint(previewPoints[i]);
				objectFactory->addDrawable(points[i]);
			}
		}
		objectFactory->addDrawable(
					objectFactory->makeLine(points[0],points[1])
					);

		clickCounter = 1;
		previewPoints[0]->setLocation(points[1]->getLocation());
		clickedPoints[0] = points[1];
		clickedPoints[1] = nullptr;
	}
}

void LineTool::resetTool()
{
	clickCounter = 0;
	previewPoints[0]->setHidden(true);
	previewPoints[1]->setHidden(true);
	linePreview->setHidden(true);

	this->clickedPoints[0] = nullptr;
	this->clickedPoints[1] = nullptr;
}

void LineTool::mouseMoveEvent(QPointF pos)
{
	if(clickCounter == 1)
	{
		previewPoints[1]->setLocation(pos);
	}
}
