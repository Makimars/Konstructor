#include "LockPositionTool.h"

LockPositionTool *LockPositionTool::instance = nullptr;

LockPositionTool::LockPositionTool()
{
	toolTips.append(tr("Select point to be locked/unlocked."));
}

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
