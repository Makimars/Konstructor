#include "Polygon.h"

Polygon::Polygon(QPolygonF polygon)
{
	this->setText("Polygon");

	std::vector<double> coordinates;
	std::vector<QPointF> points = polygon.toStdVector();

	for(uint32_t i = 0; i < points.size(); i++)
	{
		coordinates.push_back(points.at(i).x() / Settings::planeToSpaceRatio);
		coordinates.push_back(points.at(i).y() / Settings::planeToSpaceRatio);

		baseEdgeVertexes.push_back(Vertex(QVector3D(points.at(i).x() / Settings::planeToSpaceRatio, points.at(i).y() / Settings::planeToSpaceRatio, 0)));
	}

	delaunator::Delaunator d(coordinates);
	for(std::size_t i = 0; i < d.triangles.size(); i+=3)
	{
		vertexData.push_back(Vertex(QVector3D(
			d.coords[2 * d.triangles[i]],
			d.coords[2 * d.triangles[i] + 1],
			0
		)));
		vertexData.push_back(Vertex(QVector3D(
			d.coords[2 * d.triangles[i + 1]],
			d.coords[2 * d.triangles[i + 1] + 1],
			0
		)));
		vertexData.push_back(Vertex(QVector3D(
			d.coords[2 * d.triangles[i + 2]],
			d.coords[2 * d.triangles[i + 2] + 1],
			0
		)));
	}

	for(uint32_t i = 0; i < vertexData.size(); i++)
	{
		vertexData.at(i).setColor(QVector3D(0.1, 0.1, 0.1));
	}

	baseVertexes = vertexData;
}

std::vector<Vertex> *Polygon::getVertexData()
{
	return &vertexData;
}

void Polygon::setColor(QVector3D color)
{
	for(uint32_t i = 0; i < vertexData.size(); i++)
	{
		vertexData.at(i).setColor(color);
	}
}

void Polygon::setHidden(bool hidden)
{
	this->hidden = hidden;
}

bool Polygon::isHidden()
{
	return hidden;
}

int Polygon::size()
{
	return vertexData.size();
}

std::vector<Vertex> *Polygon::getOuterPoints()
{
	return &baseEdgeVertexes;
}

Vertex Polygon::getVertexAt(int i)
{
	return vertexData.at(i);
}
