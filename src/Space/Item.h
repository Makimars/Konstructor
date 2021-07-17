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

	void setDataSize(int size);
	int getDataSize();
	bool isExtruded() const;
	bool isAdditive() const;
	QVector3D getColor();

	void setSketch(QString sketch);
	QString getSketch();
	Plane *getPlane(int index);
	Plane *getParentPlane();

	void setExtrusion();
	void setExtrusion(Extrusion extrusion, Polygon *targetPolygon);
	void reverseExtrusion();
	Extrusion getExtrusion();

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

	//size of extruded vertexes in the global buffer
	int extrudedDataSize;

	QString basePlaneId;

	QVector3D globalVertexPos(uint32_t i);
	void addPlane(int index, QVector3D position, QQuaternion rotation);
	void addPlane(int index, QVector3D position, QQuaternion rotation, std::vector<QVector3D> existingVertexes);

signals:
	void updateData();

	void deletePlane(Plane *plane);
	void planeAdded(Plane *plane);

	Plane *getOriginalPlane();
};

inline QString Item::getSketch() { return sketch; }
inline QMatrix4x4 Item::toMatrix(){ return basePlane->toMatrix(); }

#endif // ITEM_H
