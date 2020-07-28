#ifndef POLYGON_H
#define POLYGON_H

#include <QListWidgetItem>

#include <QDebug>

#include "Vertex.h"

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
	Polygon(std::vector<Vertex> outerPoints, std::vector<Vertex> vertexData);

	std::vector<Vertex> *getOuterPoints();
	std::vector<Vertex> *getVertexData();

	void setColor(QVector3D color);
	void setHidden(bool hidden);
	bool isHidden();

	void extrude(double length, bool extrusion, ExtrusionDirection direction);

private:
	std::vector<Vertex> outerPoints;
	std::vector<Vertex> vertexData;

	bool hidden = false;
signals:
	void sizeChanged();
};

#endif // POLYGON_H
