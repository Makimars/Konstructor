#include "LineAdapter.h"

LineAdapter *LineAdapter::fromDrawableLine(Line *line, QVector<PointAdapter*> *transferPointsList)
{
	PointAdapter *startPoint, *endPoint;

	foreach(PointAdapter *point, *transferPointsList)
	{
		if(point->getOriginalPoint() == line->getStartPoint())
			startPoint = point;

		if(point->getOriginalPoint() == line->getEndPoint())
			endPoint = point;
	}

	return new LineAdapter(startPoint, endPoint);
}

LineAdapter::LineAdapter(){}

LineAdapter::LineAdapter(PointAdapter *pointOne, PointAdapter *pointTwo)
{
	points[0] = pointOne;
	points[1] = pointTwo;
}

bool LineAdapter::hasPoint(PointAdapter *point)
{
	if(point == points[0] | point == points[1]) return true;

	return false;
}

PointAdapter *LineAdapter::getOtherPoint(PointAdapter *point)
{
	if(points[0] == point)
		return points[1];
	if(points[1] == point)
		return points[0];

	return nullptr;
}
