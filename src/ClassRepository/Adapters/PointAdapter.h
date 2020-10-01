#ifndef TRANSFERPOINT_H
#define TRANSFERPOINT_H

#include <QVector>
#include <QPointF>

class PointAdapter
{
public:

	PointAdapter();
	PointAdapter(double x, double y);

	double getX() const;
	double getY() const;
	int getNeighborCount() const;
	QVector<PointAdapter*> getNeighborPoints() const;

	void addNeighbor(PointAdapter *point);
	bool isNeighbor(PointAdapter *point);
	void removeNeigbor(PointAdapter *point);

	QPointF toPoint();


private:
	double x,y;
	QVector<PointAdapter*> neighborPoints;
};

inline double PointAdapter::getX() const { return x; }
inline double PointAdapter::getY() const { return y; }

#endif // TRANSFERPOINT_H
