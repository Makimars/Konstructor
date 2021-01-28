#include "LockPositionTool.h"

LockPositionTool *LockPositionTool::instance = nullptr;

LockPositionTool *LockPositionTool::getInstance()
{
    if(LockPositionTool::instance == nullptr)
	    LockPositionTool::instance = new LockPositionTool();
    return LockPositionTool::instance;
}

void LockPositionTool::click(DrawableObject *clickedObject, QPointF pos)
{
    if(Point *point = dynamic_cast<Point*>(clickedObject))
    {
	if(point->isLocked()) point->setLocked(false);
	else point->setLocked(true);
    }
}

void LockPositionTool::resetTool(){}

void LockPositionTool::mouseMoveEvent(QPointF pos){}

LockPositionTool::LockPositionTool(){}
