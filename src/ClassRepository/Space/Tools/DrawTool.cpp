#include "DrawTool.h"

DrawTool *DrawTool::instance = new DrawTool();

DrawTool *DrawTool::getInstance()
{
	return instance;
}

void DrawTool::click(QPoint pos)
{
	planePosition = QVector3D(1,1,1);
	planeVector = QVector3D(0,0,0);
	emit requestDrawing();
}

void DrawTool::resetTool()
{

}

DrawTool::DrawTool()
{

}

void DrawTool::recieveDrawing(QVector<DrawableObject*> drawing)
{
	Item *item = new Item(drawing, planePosition, planeVector);
	emit addItem(item);
}
