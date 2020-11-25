#ifndef ITEM_H
#define ITEM_H

#include "Plane.h"
#include "Polygon.h"

class Item : public QObject, public QTreeWidgetItem
{
	Q_OBJECT
public:
	Item(Space::Plane *plane, std::vector<QPolygonF> polygons, QString sketch);

	void copyVertexesToReference(std::vector<Vertex*> vector, int itemIndex);
	void setPolygons(std::vector<QPolygonF> polygons);
	std::vector<Polygon*> *getPolygons();
	void setSketch(QString sketch);
	QString getSketch();
	int getItemIndex();
	int size();
	bool isExtruded();

	void extrude(Extrusion extrusion, Polygon *targetPolygon);

	QMatrix4x4 toMatrix();

private:
	Space::Plane *basePlane;
	QVector<Space::Plane> planes;
	QString sketch;

	//pointers to the global buffer
	std::vector<Vertex*> targetVertexBuffer;
	//original polygons
	std::vector<Polygon*> polygons;

	//extrusion info
	bool extruded = false;
	Extrusion extrusion;
	Polygon *extrudedPolygon;

	//extruded vertex data
	std::vector<Vertex> vertexBuffer;

	int itemIndex;

signals:
	void updateData();

};

inline QString Item::getSketch() { return sketch; }
inline int Item::getItemIndex() { return itemIndex; }
inline QMatrix4x4 Item::toMatrix(){ return basePlane->toMatrix(); }

#endif // ITEM_H
