#include "LineCenterTool.h"

LineCenterTool *LineCenterTool::instance = nullptr;

LineCenterTool *LineCenterTool::getInstance()
{
	if(LineCenterTool::instance == nullptr)
		LineCenterTool::instance = new LineCenterTool();
	return LineCenterTool::instance;
}

LineCenterTool::LineCenterTool() : Tool(){}

void LineCenterTool::click(DrawableObject *clickedObject, QPointF pos)
{
	if(clickedObject != nullptr)
	{
		if(clickedObject->getType() == Global::Line)
		{
			Point *point = objectFactory->makePoint(pos.x(), pos.y());
			objectFactory->addDrawable(point);
			Line *line = dynamic_cast<Line*>(clickedObject);

			objectFactory->addDrawable(
						objectFactory->makeLineCenterPointConstraint(line, point)
						);
		}
	}
}

void LineCenterTool::resetTool(){}

