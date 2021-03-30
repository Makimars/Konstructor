#include "ArcTool.h"

ArcTool *ArcTool::instance = nullptr;

ArcTool::ArcTool() : Tool()
{
	//preview
	for(int i = 0; i < 3; i++)
	{
		this->previewPoints[i] = planeFactory->makePoint();
		this->previewPoints[i]->setHidden(true);
		planeFactory->addToScene(previewPoints[i]);
	}

	this->preview = this->planeFactory->makeArc(previewPoints);
	this->preview->setHidden(true);
	planeFactory->addToScene(preview);
}

ArcTool *ArcTool::getInstance()
{
	if(ArcTool::instance == nullptr)
		ArcTool::instance = new ArcTool();
	return ArcTool::instance;
}

void ArcTool::click(DrawableObject *clickedObject, QPointF pos)
{
	clickedPoints[clickCounter] = dynamic_cast<Point*>(clickedObject);
	clickCounter++;

	if(clickCounter == 1)
	{
		if(clickedPoints[0] != nullptr)
			previewPoints[0]->setLocation(clickedPoints[0]->getLocation());
		else
			this->previewPoints[0]->setLocation(pos);

		this->previewPoints[0]->setHidden(false);
	}
	else if (clickCounter == 2)
	{
		if(clickedPoints[1] != nullptr)
			previewPoints[1]->setLocation(clickedPoints[1]->getLocation());
		else
			this->previewPoints[1]->setLocation(pos);

		this->previewPoints[1]->setHidden(false);
		this->preview->setHidden(false);
		this->previewPoints[2]->setHidden(false);
	}
	else if (clickCounter == 3)
	{
		mouseMoveEvent(pos);

		Point *newPoints[3];
		for(int i = 0; i < 3; i++)
		{
			if(clickedPoints[i] != nullptr)
			{
				newPoints[i] = clickedPoints[i];
			}
			else
			{
				newPoints[i] = planeFactory->copyPoint(previewPoints[i]);
				planeFactory->addDrawable(newPoints[i]);
			}
		}

		planeFactory->addDrawable(
					planeFactory->makeArc(newPoints)
					);

		resetTool();
	}
	else
	{
		clickCounter = 0;
	}
}

void ArcTool::resetTool()
{
	for(int i = 0; i < 3; i++)
		this->previewPoints[i]->setHidden(true);
	this->preview->setHidden(true);

	clickCounter = 0;
}

void ArcTool::mouseMoveEvent(QPointF pos)
{
	switch (clickCounter) {
		case 1:
			previewPoints[1]->setLocation(pos);
			break;
		case 2:
			previewPoints[2]->setLocation(pos);
			break;
		default:
			break;
	}
}
