#include "LineCenterTool.h"

LineCenterTool *LineCenterTool::instance = nullptr;

LineCenterTool *LineCenterTool::getInstance()
{
	if(LineCenterTool::instance == nullptr)
		LineCenterTool::instance = new LineCenterTool();
	return LineCenterTool::instance;
}

LineCenterTool::LineCenterTool() : Tool()
{
	toolTips.append(tr("Select a line to create center."));
}

void LineCenterTool::click(DrawableObject *clickedObject, QPointF pos)
{
	if(clickedObject != nullptr)
	{
		if(clickedObject->getType() == Global::Line)
		{
			Point *point = planeFactory->makePoint(pos.x(), pos.y());
			planeFactory->addDrawable(point);
			Line *line = dynamic_cast<Line*>(clickedObject);

			planeFactory->addDrawable(
						planeFactory->makeLineCenterPointConstraint(line, point)
						);
		}
	}

}
