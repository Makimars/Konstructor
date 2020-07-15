#include "DrawTool.h"

DrawTool *DrawTool::instance = new DrawTool();

DrawTool *DrawTool::getInstance()
{
	return instance;
}

DrawTool::DrawTool()
{

}

void DrawTool::recieveDrawing(QVector<DrawableObject*> drawing)
{
	QTreeWidgetItem *item = emit getPlane();

	if(Space::Plane *plane = dynamic_cast<Space::Plane*>(item))
	{
		if(drawing.size() > 0)
		{
			Item *item = new Item(drawing, plane);
			emit addItem(item);
		}
	}
}
