#ifndef POLYGONATOR_H
#define POLYGONATOR_H

#include "LineAdapter.h"

class Polygonator : public QObject
{
	Q_OBJECT
public:
	static Polygonator *getInstance();

private:
	Polygonator();
	static Polygonator *instance;

	void convertToIntermediate(QVector<DrawableObject*> drawing, QVector<PointAdapter*> *transferPoints, QVector<LineAdapter*> *transferLines);
	void assignNeigbors(QVector<PointAdapter*> *transferPoints, QVector<LineAdapter*> *transferLines);
	std::vector<QPolygonF> generatePolygons(QVector<PointAdapter*> *transferPoints);

public slots:
	void recieveDrawing(QVector<DrawableObject*> drawing);

signals:
	void sendPolygons(std::vector<QPolygonF> polygons, QString sketch);
};

#endif // POLYGONATOR_H
