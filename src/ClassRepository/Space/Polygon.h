#ifndef POLYGON_H
#define POLYGON_H

#include <QListWidgetItem>

#include <QDebug>

#include "src/ClassRepository/Settings.h"
#include "Vertex.h"
#include "include/delaunator/delaunator.h"

enum ExtrusionDirection
{
	Front,
	FrontAndBack,
	Back
};

class Polygon : public QObject, public QListWidgetItem
{
	Q_OBJECT
public:
	Polygon(QPolygonF polygon);

	std::vector<Vertex> *getOuterPoints();
	std::vector<Vertex> *getVertexData();

	void setColor(QVector3D color);
	void setHidden(bool hidden);
	bool isHidden();
	int size();

	void extrude(double length, bool extrusion, ExtrusionDirection direction);

private:
	std::vector<Vertex> outerVertexes;
	std::vector<Vertex> vertexData;

	bool hidden = false;
signals:
	void updateData();
};

#endif // POLYGON_H
