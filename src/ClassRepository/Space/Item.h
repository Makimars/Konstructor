#ifndef ITEM_H
#define ITEM_H

#include <nlohmann/json.hpp>

#include "Plane.h"
#include "Polygon.h"

class Item : public QObject, public QTreeWidgetItem
{
	Q_OBJECT
public:
	Item(Plane *plane, std::vector<QPolygonF> polygons, QString sketch);
	~Item();

	void copyVertexesToReference(std::vector<Vertex*> vector, int itemIndex);
	void setPolygons(std::vector<QPolygonF> polygons);
	QVector<Polygon*> *getPolygons();
	void setSketch(QString sketch);
	QString getSketch();
	int getItemIndex();
	int size();
	bool isExtruded();

	void extrude(Extrusion extrusion, Polygon *targetPolygon);
	nlohmann::json toJson();

	QMatrix4x4 toMatrix();

private:
	Plane *basePlane;
	std::vector<Plane*> planes;
	QString sketch;

	//pointers to the global buffer
	std::vector<Vertex*> targetVertexBuffer;
	//original polygons
	QVector<Polygon*> polygons;

	//extrusion info
	bool extruded = false;
	Extrusion extrusion;
	Polygon *extrudedPolygon;

	//extruded vertex data
	std::vector<Vertex> vertexBuffer;

	int itemIndex;

	void addPlane(int index, QVector3D position, QQuaternion rotation);

signals:
	void updateData();

	void deletePlane(Plane *plane);
	void planeAdded(Plane *plane);
};

inline QString Item::getSketch() { return sketch; }
inline int Item::getItemIndex() { return itemIndex; }
inline QMatrix4x4 Item::toMatrix(){ return basePlane->toMatrix(); }

#endif // ITEM_H
