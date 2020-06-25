#include "TransferLine.h"


TransferLine *TransferLine::fromDrawableLine(Line *line, QVector<TransferPoint*> *transferPointsList)
{
	TransferPoint *startPoint, *endPoint;

	foreach(TransferPoint *point, *transferPointsList)
	{
		if(point->getOriginalPoint() == line->getStartPoint())
			startPoint = point;

		if(point->getOriginalPoint() == line->getEndPoint())
			endPoint = point;
	}

	return new TransferLine(startPoint, endPoint);
}

TransferLine::TransferLine(){}

TransferLine::TransferLine(TransferPoint *pointOne, TransferPoint *pointTwo)
{
	points[0] = pointOne;
	points[1] = pointTwo;
}

bool TransferLine::hasPoint(TransferPoint *point)
{
	if(point == points[0] | point == points[1]) return true;

	return false;
}

TransferPoint *TransferLine::getOtherPoint(TransferPoint *point)
{
	if(points[0] == point)
		return points[1];
	if(points[1] == point)
		return points[0];

	return nullptr;
}
