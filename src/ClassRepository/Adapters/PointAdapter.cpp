#include "PointAdapter.h"

PointAdapter::PointAdapter(){}

PointAdapter::PointAdapter(double x, double y)
{
	this->x = x;
	this->y = y;
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
