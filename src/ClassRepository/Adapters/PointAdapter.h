#ifndef TRANSFERPOINT_H
#define TRANSFERPOINT_H

#include <vector>

#include "../Plane/Plane"

class PointAdapter
{
public:
	static PointAdapter *fromDrawablePoint(Point *point);

	PointAdapter();
	PointAdapter(double x, double y, Point *point);

	double getX() const;
	double getY() const;
	Point *getOriginalPoint() const;
	int getNeighborCount() const;
	QVector<PointAdapter *> getNeighborPoints() const;

	void addNeighbor(PointAdapter *point);
	bool isNeighbor(PointAdapter *point);

	QPointF toPoint();


private:
	double x,y;
	QVector<PointAdapter*> neighborPoints;

	Point *originalPoint;
};

inline double PointAdapter::getX() const { return x; }
inline double PointAdapter::getY() const { return y; }

#endif // TRANSFERPOINT_H
