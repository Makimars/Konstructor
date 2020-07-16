#ifndef TRANSFERPOINT_H
#define TRANSFERPOINT_H

#include <vector>

#include "../Item.h"

class TransferPoint
{
public:
	static TransferPoint *fromDrawablePoint( Point *point);

	TransferPoint();
	TransferPoint(double x, double y, Point *point);

	double getX() const;
	double getY() const;
	Point *getOriginalPoint() const;

	void addNeighbor(TransferPoint *point);
	bool isNeighbor(TransferPoint *point);

private:
	double x,y;
	QVector<TransferPoint*> neighborPoints;

	Point *originalPoint;
};

inline double TransferPoint::getX() const { return x; }
inline double TransferPoint::getY() const { return y; }

#endif // TRANSFERPOINT_H
