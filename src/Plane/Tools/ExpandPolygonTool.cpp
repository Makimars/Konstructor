#include "ExpandPolygonTool.h"

ExpandPolygonTool *ExpandPolygonTool::instance = nullptr;

ExpandPolygonTool::ExpandPolygonTool() : Tool()
{
	previewCircleCenter = planeFactory->makePoint(0,0);
	previewCircleCenter->setHidden(true);
	planeFactory->addToScene(previewCircleCenter);

	previewCircle = planeFactory->makeCircle(previewCircleCenter);
	previewCircle->setHidden(true);
	planeFactory->addToScene(previewCircle);

	clickedCircle = nullptr;

	toolTips.append(tr("Select circle or a polygon."));
	toolTips.append(tr("Select size"));
}

ExpandPolygonTool *ExpandPolygonTool::getInstance()
{
	if(ExpandPolygonTool::instance == nullptr)
		ExpandPolygonTool::instance = new ExpandPolygonTool();
	return ExpandPolygonTool::instance;
}

void ExpandPolygonTool::click(DrawableObject *clickedObject, QPointF pos)
{
	clickCounter++;

	if(clickCounter == 1)
	{
		if(clickedObject == nullptr)
		{
			clickCounter = 0;
			return;
		}

		if(clickedObject->getType() == Global::Types::Circle)
		{
			previewCircle->setHidden(false);
			previewCircleCenter->setHidden(false);

			clickedCircle = dynamic_cast<Circle*>(clickedObject);

			previewCircleCenter->setLocation(clickedCircle->getCenterPoint()->getLocation());
			previewCircle->setRadius(clickedCircle->getRadius());
		}
		else if(clickedObject->getType() == Global::Types::Line)
		{
			//find polygon from lineStartPoint and expand
		}
		else if(clickedObject->getType() == Global::Types::Point)
		{
			//find polygon from point and expand
		}
	}
	else if(clickCounter == 2)
	{
		if((!previewCircle->isHidden()) | (clickedCircle != nullptr))
		{
			Circle *circle = planeFactory->makeCircle(clickedCircle->getCenterPoint());
			circle->setRadius(clickedCircle->getCenterPoint()->distanceFrom(pos));

			planeFactory->addDrawable(circle);
			resetTool();
		}
	}
}

void ExpandPolygonTool::resetTool()
{
	previewCircle->setHidden(true);
	previewCircleCenter->setHidden(true);

	clickedCircle = nullptr;
}

void ExpandPolygonTool::mouseMoveEvent(QPointF pos)
{
	if(!previewCircle->isHidden())
		previewCircle->setRadius(previewCircleCenter->distanceFrom(pos));
}

