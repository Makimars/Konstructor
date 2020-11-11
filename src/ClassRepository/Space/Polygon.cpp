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
	emit updateData();
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

void Polygon::extrude()
{
	double length = extrusion.length;

	if(extrusion.direction == ExtrusionDirection::Back)
	{
		length = -length;
	}
	else if (extrusion.direction == ExtrusionDirection::FrontAndBack)
	{
		length *= 0.5;

		for (int i = 0; i < vertexData.size(); i++)
		{
			vertexData.at(i).setZ(-length);
		}
		for (int i = 0; i < baseEdgeVertexes.size(); i++)
		{
			baseEdgeVertexes.at(i).setZ(-length);
		}
	}

	int count = vertexData.size();
	for (int i = 0; i < count; i++)
	{
		Vertex vertex = baseVertexes.at(i);
		vertex.setPosition(QVector3D(vertex.position().x(), vertex.position().y(), length));

		vertexData.push_back(vertex);
	}

	//edges
	for (int i = 0; i < baseEdgeVertexes.size() - 1; i++)
	{
		Vertex copyVertex;

		vertexData.push_back(baseEdgeVertexes.at(i));
		vertexData.push_back(baseEdgeVertexes.at(i + 1));
		copyVertex = baseEdgeVertexes.at(i);
		copyVertex.setZ(length);
		vertexData.push_back(copyVertex);

		copyVertex = baseEdgeVertexes.at(i);
		copyVertex.setZ(length);
		vertexData.push_back(copyVertex);
		vertexData.push_back(baseEdgeVertexes.at(i + 1));
		copyVertex = baseEdgeVertexes.at(i + 1);
		copyVertex.setZ(length);
		vertexData.push_back(copyVertex);
	}

	{
		Vertex copyVertex;
		int lastOuterIndex = baseEdgeVertexes.size() - 1;

		vertexData.push_back(baseEdgeVertexes.at(lastOuterIndex));
		vertexData.push_back(baseEdgeVertexes.at(0));
		copyVertex = baseEdgeVertexes.at(lastOuterIndex);
		copyVertex.setZ(length);
		vertexData.push_back(copyVertex);

		copyVertex = baseEdgeVertexes.at(lastOuterIndex);
		copyVertex.setZ(length);
		vertexData.push_back(copyVertex);
		vertexData.push_back(baseEdgeVertexes.at(0));
		copyVertex = baseEdgeVertexes.at(0);
		copyVertex.setZ(length);
		vertexData.push_back(copyVertex);
	}

	emit updateData();
}

void Polygon::setExtrusion(Extrusion extrusion)
{
	this->extrusion = extrusion;
}

std::vector<Vertex> *Polygon::getOuterPoints()
{
	return &baseEdgeVertexes;
}
