#include "ArcTool.h"

ArcTool *ArcTool::instance = nullptr;

DimensionTool *ArcTool::getInstance()
{

}

void ArcTool::click(DrawableObject *clickedObject, QPointF pos)
{

}

void ArcTool::resetTool()
{

}

ArcTool::ArcTool() : Tool()
{
	//preview
	this->preview = this->objectFactory->makeArc(previewPoints);

}
