#include "Polygon.h"

Polygon::Polygon(QPolygonF polygon)
{
	this->setText("Polygon");

	for (int i = 0; i < polygon.size(); i++ )
	{
		this->polygon.push_back(QPointF(
									polygon.at(i).x() / Settings::planeToSpaceRatio,
									polygon.at(i).y() / Settings::planeToSpaceRatio
									));
	}
}

std::vector<QPointF> Polygon::getPoints()
{
	return polygon.toStdVector();
}

QPolygonF Polygon::getQpolygon()
{
	return polygon;
}

int Polygon::getDataSize() const
{
	return dataSize;
}

void Polygon::setDataSize(int value)
{
	dataSize = value;
}

void Polygon::setColor(QVector3D color)
{
	this->color = color;
}

QVector3D Polygon::getColor()
{
	return this->color;
}

void Polygon::setHidden(bool hidden)
{
	this->hidden = hidden;
}

bool Polygon::isHidden()
{
	return hidden;
}
