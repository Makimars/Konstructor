#include "DrawTool.h"

DrawTool *DrawTool::instance = new DrawTool();

DrawTool *DrawTool::getInstance()
{
	return instance;
}

DrawTool::DrawTool()
{

}

std::vector<Vertex> DrawTool::generateVertexes(QPolygonF polygon, std::vector<Vertex> *outerVertexes)
{
	std::vector<Vertex> planeVertexes;

	std::vector<double> coordinates;
	std::vector<QPointF> points = polygon.toStdVector();

	for(uint32_t i = 0; i < points.size(); i++)
	{
		coordinates.push_back(points.at(i).x() / Settings::planeToSpaceRatio);
		coordinates.push_back(points.at(i).y() / Settings::planeToSpaceRatio);

		outerVertexes->push_back(Vertex(QVector3D(points.at(i).x(), points.at(i).y(), 0)));
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

	for(uint32_t i = 0; i < planeVertexes.size(); i++)
	{
		planeVertexes.at(i).setColor(QVector3D(0.1, 0.1, 0.1));
	}
	return planeVertexes;
}

void DrawTool::recievePolygons(std::vector<QPolygonF> polygons, QString sketch)
{
	if(polygons.size() > 0)
	{
		std::vector<Polygon*> itemPolygons;

		foreach(QPolygonF polygon, polygons)
		{
			if(polygon.size() > 0)
			{
				std::vector<Vertex> outerVertexes;
				std::vector<Vertex> vertexData = generateVertexes(polygon, &outerVertexes);

				itemPolygons.push_back(new Polygon(outerVertexes, vertexData));
			}
		}

		if(Space::Plane *plane = dynamic_cast<Space::Plane*>(targetItem))
		{
			Item *item = new Item(plane, sketch);
			item->setPolygons(itemPolygons);
			emit addItem(item);
		}
		else if(Item *existingItem = dynamic_cast<Item*>(targetItem))
		{
			existingItem->setPolygons(itemPolygons);
		}
	}
}
