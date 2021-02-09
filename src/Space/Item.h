#ifndef ITEM_H
#define ITEM_H

#include "include/nlohmann/json.hpp"

#include "Plane.h"
#include "Polygon.h"

class Item : public QObject, public QTreeWidgetItem
{
	Q_OBJECT
public:
	Item();
	Item(Plane *plane, std::vector<QPolygonF> polygons, QString sketch);
	~Item();

	void setPolygons(std::vector<QPolygonF> polygons);
	QVector<Polygon*> *getPolygons();

	void setItemIndex(int index);
	int getItemIndex();
	void setDataSize(int size);
	int getDataSize();
	bool isExtruded();
	QVector3D getColor();

	void setSketch(QString sketch);
	QString getSketch();
	Plane *getPlane(int index);

	void extrude();
	void extrude(Extrusion extrusion, Polygon *targetPolygon);

	std::vector<Vertex> *getExtrudedVertexes();

	nlohmann::json toJson();
	void loadData(QString basePlaneId, Extrusion extrusion, bool extruded, int extrudedPolygon);
	void loadRelations(std::vector<Item*> list);

	QMatrix4x4 toMatrix();

private:
	Plane *basePlane;
	std::vector<Plane*> planes;
	QString sketch;

	//original polygons
	QVector<Polygon*> polygons;

	//extruded vertex data
	std::vector<Vertex> extrudedVertexes;

	//extrusion info
	bool extruded = false;
	Extrusion extrusion;
	Polygon *extrudedPolygon;

	//item's position within the global buffer
	int itemIndex;
	//size of extruded vertexes in the global buffer
	int extrudedDataSize;

	QString basePlaneId;

	QVector3D globalVertexPos(u_int32_t i);
	void addPlane(int index, QVector3D position, QQuaternion rotation);
	void addPlane(int index, QVector3D position, QQuaternion rotation, std::vector<QVector3D> existingVertexes);

signals:
	void updateData();

	void deletePlane(Plane *plane);
	void planeAdded(Plane *plane);

	Plane *getBasePlane();
};

inline QString Item::getSketch() { return sketch; }
inline int Item::getItemIndex() { return itemIndex; }
inline QMatrix4x4 Item::toMatrix(){ return basePlane->toMatrix(); }

#endif // ITEM_H
