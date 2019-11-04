#include "Tool.h"

void Tool::resetTool()
{

}

//----------     getters and setters     ---------
QBrush * Tool::getCurrentBrush() const
{
	return current_brush;
}

void Tool::setCurrentBrush(QBrush * value)
{
	current_brush = value;
}

QPen * Tool::getCurrentPen() const
{
	return current_pen;
}

void Tool::setCurrentPen(QPen * value)
{
	current_pen = value;
}

Tool::Tool()
{

}
