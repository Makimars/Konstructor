#include "Tool.h"

void Tool::resetTool()
{

}

//----------     getters and setters     ---------
QBrush * Tool::getCurrentBrush() const
{
	return currentBrush;
}

void Tool::setCurrentBrush(QBrush * value)
{
	currentBrush = value;
}

QPen * Tool::getCurrentPen() const
{
	return currentPen;
}

void Tool::setCurrentPen(QPen * value)
{
	currentPen = value;
}

Tool::Tool()
{

}
