#ifndef POLYGONATOR_H
#define POLYGONATOR_H

#include <queue>

#include "../Plane/Plane.h"
#include "PointAdapter.h"

class Polygonator : public QObject
{
	Q_OBJECT
public:
	static Polygonator *getInstance();

	std::vector<QPolygonF> generatePolygons(QVector<DrawableObject*> drawing);

private:
	Polygonator();
	static Polygonator *instance;

	QVector<PointAdapter*> generateAdapters(QVector<DrawableObject*> drawing);
	QVector<QPolygonF> generatePolygons(QVector<PointAdapter*> transferPoints);

	PointAdapter *getAdapterRepresentation(Point *point);
};

#endif // POLYGONATOR_H
