#include "TransferPoint.h"

TransferPoint *TransferPoint::fromDrawablePoint(Point *point)
{
	return new TransferPoint(
				point->getX() / Settings::planeToSpaceRatio,
				point->getY() / Settings::planeToSpaceRatio,
				point
				);
}

TransferPoint::TransferPoint(){}

TransferPoint::TransferPoint(double x, double y, Point *point)
{
	this->x = x;
	this->y = y;
	originalPoint = point;
}

Point *TransferPoint::getOriginalPoint() const
{
	return originalPoint;
}

void TransferPoint::addNeighbor(TransferPoint *point)
{
	if(point != nullptr)
	{
		if(!isNeighbor(point))
			neighborPoints.push_back(point);
	}
}

bool TransferPoint::isNeighbor(TransferPoint *point)
{
	for(int i = 0; i < neighborPoints.size(); i++)
	{
		if(neighborPoints.at(i) == point) return true;
	}

	return false;
}
