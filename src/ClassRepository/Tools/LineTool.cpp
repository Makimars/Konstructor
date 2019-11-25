#include "LineTool.h"

LineTool *LineTool::instance = nullptr;

LineTool::LineTool(Point *mousePoint, QGraphicsScene *scene)
{
	this->objectFactory = DrawablesFactory::getInstance();

	//line preview
	this->linePreviewStartPoint = this->objectFactory
			->makePoint();

	this->linePreview = this->objectFactory
			->makeLine(this->linePreviewStartPoint, mousePoint);
	this->linePreview->setHidden(true);

	this->lineLenghtDimension = this->objectFactory
			->makeDimension(this->linePreview, 0);
	this->lineLenghtDimension->setHidden(false);

	scene->addItem(this->linePreview);
	scene->addItem(this->lineLenghtDimension);

}

void LineTool::initialise(Point *mousePoint,
							QGraphicsScene *scene,
							QBrush *defaultBrush,
							QPen *defaultPen)
{
	if(LineTool::instance == nullptr)
		LineTool::instance = new LineTool(mousePoint, scene);

	LineTool::instance->setCurrentPen(defaultPen);
	LineTool::instance->setCurrentBrush(defaultBrush);
}

LineTool *LineTool::getInstance()
{
	return LineTool::instance;
}

void LineTool::click(DrawableObject *clickedObject, Point *mousePoint)
{
	if(clickedObject == nullptr)
		clickedObject = mousePoint->clone();

	if(clickedObject->getType() == TYPE_POINT)
	{
		Point *clickedPoint;

		if(clickedObject != nullptr)
			clickedPoint = dynamic_cast<Point*>(clickedObject);
		else
			clickedPoint = mousePoint->clone();

		this->clickedPoints[1] = this->clickedPoints[0];
		this->clickedPoints[0] = clickedPoint;

		this->linePreview->setHidden(false);
		this->linePreviewStartPoint->setLocation(this->clickedPoints[0]->getLocation());

		if(this->clickedPoints[1] != nullptr)
		{
			this->objectFactory->addDrawable(this->clickedPoints[0]);
			this->objectFactory->addDrawable(this->clickedPoints[1]);

			Line *line = this->objectFactory
					->makeLine(this->clickedPoints[0], this->clickedPoints[1]);
			this->objectFactory->addDrawable(line);

			this->clickedPoints[1] = nullptr;
		}
	}
}

void LineTool::resetTool()
{
	this->objectFactory->tryDeleteDrawable(this->clickedPoints[0]);
	this->clickedPoints[0] = nullptr;

	this->objectFactory->tryDeleteDrawable(this->clickedPoints[1]);
	this->clickedPoints[1] = nullptr;

	this->linePreview->setHidden(true);
}

void LineTool::KeyPressed(QKeyEvent *event)
{
	bool ok = false;
	event->text().toInt(&ok);
	if(ok)
	{

	}
}
