#include "DrawTool.h"

DrawTool *DrawTool::instance = new DrawTool();

DrawTool *DrawTool::getInstance()
{
	return instance;
}

DrawTool::DrawTool()
{

}

std::vector<Vertex> DrawTool::generateVertexes(QVector<DrawableObject*> drawing)
{
	QVector<TransferPoint*> transferPoints;
	QVector<TransferLine*> transferLines;

	//convert to intermediate lines and points
	convertToIntermediate(drawing, &transferPoints, &transferLines);

	//assign neigbors to points
	assignNeigbors(&transferPoints, &transferLines);

	//convert to triangles
	return triangularize(&transferPoints);
}

void DrawTool::convertToIntermediate(QVector<DrawableObject*> drawing, QVector<TransferPoint*> *transferPoints, QVector<TransferLine*> *transferLines)
{
	//generate points
	foreach (DrawableObject *obj, drawing)
	{
		if((obj->getType() == Global::Point) & !obj->isConstructional() & !obj->isHidden())
		{
			Point *point = dynamic_cast<Point*>(obj);

			transferPoints->push_back(TransferPoint::fromDrawablePoint(point));
		}
	}

	//generate lines from Drawable lines
	foreach (DrawableObject *obj, drawing)
	{
		if((obj->getType() == Global::Line) & !obj->isConstructional() & !obj->isHidden())
		{
			Line *line = dynamic_cast<Line*>(obj);

			transferLines->push_back(TransferLine::fromDrawableLine(line, transferPoints));
		}
	}

	//generate lines from circles
}

void DrawTool::assignNeigbors(QVector<TransferPoint*> *transferPoints, QVector<TransferLine*> *transferLines)
{
	//for every transfer point link neighboring points from lines
	foreach(TransferPoint *point, *transferPoints)
	{
		foreach(TransferLine *line, *transferLines)
		{
			if(line->hasPoint(point))
				point->addNeighbor(line->getOtherPoint(point));
		}
	}
}

std::vector<Vertex> DrawTool::triangularize(QVector<TransferPoint*> *transferPoints)
{
	//generate vertexData from transferPoints

	std::vector<double> coordinates;
	for(int i = 0; i < transferPoints->count(); i++)
	{
		coordinates.push_back(transferPoints->at(i)->getX());
		coordinates.push_back(transferPoints->at(i)->getY());
	}

	delaunator::Delaunator d(coordinates);
	std::vector<Vertex> planeVertexes;

	for(std::size_t i = 0; i < d.triangles.size(); i+=3)
	{
		planeVertexes.push_back(Vertex(QVector3D(
			d.coords[2 * d.triangles[i]],
			d.coords[2 * d.triangles[i] + 1],
			0
		)));
		planeVertexes.push_back(Vertex(QVector3D(
			d.coords[2 * d.triangles[i + 1]],
			d.coords[2 * d.triangles[i + 1] + 1],
			0
		)));
		planeVertexes.push_back(Vertex(QVector3D(
			d.coords[2 * d.triangles[i + 2]],
			d.coords[2 * d.triangles[i + 2] + 1],
			0
		)));
	}

	for(int i = 0; i < planeVertexes.size(); i++)
	{
		planeVertexes.at(i).setColor(QVector3D(0.1, 0.1, 0.1));
	}

	return planeVertexes;
}

void DrawTool::recieveDrawing(QVector<DrawableObject*> drawing)
{
	if(Space::Plane *plane = dynamic_cast<Space::Plane*>(targetItem))
	{
		if(drawing.size() > 0)
		{
			Item *item = new Item(drawing, plane);
			item->setPlaneVertexes(generateVertexes(drawing));
			emit addItem(item);
		}
	}
	else if(Item *existingItem = dynamic_cast<Item*>(targetItem))
	{
		if(drawing.size() > 0)
		{
			existingItem->setPlaneVertexes(generateVertexes(drawing));
		}
	}
}
