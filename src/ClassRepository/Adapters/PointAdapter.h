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
	bool visited() const;
	void setVisited(bool value);
	Point *getOriginalPoint() const;
	int getNeighborCount() const;
	QVector<PointAdapter *> getNeighborPoints() const;

	void addNeighbor(PointAdapter *point);
	bool isNeighbor(PointAdapter *point);

	QPointF toPoint();


private:
	double x,y;
	bool m_visited;
	QVector<PointAdapter*> neighborPoints;

	Point *originalPoint;
};

inline double PointAdapter::getX() const { return x; }
inline double PointAdapter::getY() const { return y; }
inline bool PointAdapter::visited() const { return m_visited; }
inline void PointAdapter::setVisited(bool value) { m_visited = value; }

#endif // TRANSFERPOINT_H
