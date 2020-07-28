#include "Polygon.h"

Polygon::Polygon(std::vector<Vertex> outerPoints, std::vector<Vertex> vertexData)
{
	this->outerPoints = outerPoints;
	this->vertexData = vertexData;

	this->setText("Polygon");
}

std::vector<Vertex> *Polygon::getVertexData()
{
	return &vertexData;
}

void Polygon::setColor(QVector3D color)
{
	foreach (Vertex vertex, vertexData)
	{
		vertex.setColor(color);
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

void Polygon::extrude(double length, bool extrusion, ExtrusionDirection direction)
{

}

std::vector<Vertex> *Polygon::getOuterPoints()
{
	return &outerPoints;
}
