#include "PointAdapter.h"

PointAdapter *PointAdapter::fromDrawablePoint(Point *point)
{
	return new PointAdapter(
				point->getX() / Settings::planeToSpaceRatio,
				point->getY() / Settings::planeToSpaceRatio,
				point
				);
}

PointAdapter::PointAdapter(){}

PointAdapter::PointAdapter(double x, double y, Point *point)
{
	this->x = x;
	this->y = y;
	originalPoint = point;
}

Point *PointAdapter::getOriginalPoint() const
{
	return originalPoint;
}

int PointAdapter::getNeighborCount() const
{
	return neighborPoints.count();
}

void PointAdapter::addNeighbor(PointAdapter *point)
{
	if(point != nullptr)
	{
		if(!isNeighbor(point))
			neighborPoints.push_back(point);
	}
}

bool PointAdapter::isNeighbor(PointAdapter *point)
{
	for(int i = 0; i < neighborPoints.size(); i++)
	{
		if(neighborPoints.at(i) == point) return true;
	}

	return false;
}

void PointAdapter::removeNeigbor(PointAdapter *point)
{
	neighborPoints.removeAll(point);
}

QPointF PointAdapter::toPoint()
{
	return QPointF(x,y);
}

QVector<PointAdapter *> PointAdapter::getNeighborPoints() const
{
	return neighborPoints;
}
