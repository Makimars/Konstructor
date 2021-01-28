#ifndef POLYGONATOR_H
#define POLYGONATOR_H

#include <queue>

#include "../Plane/Plane"
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

public slots:
	void recieveDrawing(QVector<DrawableObject*> drawing);

signals:
	void sendPolygons(std::vector<QPolygonF> polygons, QString sketch);
};

#endif // POLYGONATOR_H
