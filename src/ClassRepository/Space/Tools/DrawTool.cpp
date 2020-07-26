#include "DrawTool.h"

DrawTool *DrawTool::instance = new DrawTool();

DrawTool *DrawTool::getInstance()
{
	return instance;
}

DrawTool::DrawTool()
{

}

std::vector<Vertex> DrawTool::generateVertexes(std::vector<QPolygonF> polygons)
{
	std::vector<Vertex> planeVertexes;

	foreach (QPolygonF polygon, polygons)
	{
		std::vector<double> coordinates;
		std::vector<QPointF> points = polygon.toStdVector();

		for(int i = 0; i < points.size(); i++)
		{
			coordinates.push_back(points.at(i).x() / Settings::planeToSpaceRatio);
			coordinates.push_back(points.at(i).y() / Settings::planeToSpaceRatio);
		}

		delaunator::Delaunator d(coordinates);
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


	}

	for(int i = 0; i < planeVertexes.size(); i++)
	{
		planeVertexes.at(i).setColor(QVector3D(0.1, 0.1, 0.1));
	}
	return planeVertexes;
}

void DrawTool::recievePolygons(std::vector<QPolygonF> polygons, QString sketch)
{
	if(Space::Plane *plane = dynamic_cast<Space::Plane*>(targetItem))
	{
		if(polygons.size() > 0)
		{
			Item *item = new Item(plane, sketch);
			item->setPlaneVertexes(generateVertexes(polygons));
			emit addItem(item);
		}
	}
	else if(Item *existingItem = dynamic_cast<Item*>(targetItem))
	{
		if(polygons.size() > 0)
		{
			existingItem->setPlaneVertexes(generateVertexes(polygons));
		}
	}
}
