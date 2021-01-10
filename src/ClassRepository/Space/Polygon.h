#ifndef POLYGON_H
#define POLYGON_H

#include <QListWidgetItem>

#include <QDebug>

#include "src/ClassRepository/Settings.h"
#include "Vertex.h"

enum ExtrusionDirection
{
	Front,
	FrontAndBack,
	Back
};

struct Extrusion{
	double length;
	bool additive;
	ExtrusionDirection direction;
};


class Polygon : public QListWidgetItem
{
public:
	Polygon(QPolygonF polygon);

	std::vector<QPointF> getPoints();
	int getDataSize() const;
	void setDataSize(int value);

	void setColor(QVector3D color);
	QVector3D getColor();
	void setHidden(bool hidden);
	bool isHidden();
private:
	//boundry vertex of the polygon
	QPolygonF polygon;
	//length of the processed vertexes represented by this polygon
	int dataSize;
	QVector3D color;
	bool hidden = false;
};

#endif // POLYGON_H
