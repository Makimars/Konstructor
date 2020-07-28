#ifndef ITEM_H
#define ITEM_H

#include "Plane.h"
#include "Polygon.h"

class Item : public QObject, public QTreeWidgetItem
{
	Q_OBJECT
public:
	Item(Space::Plane *plane, QString sketch);

	void setVectorReference(std::vector<Vertex*> vector, int itemIndex);
	void setPolygons(std::vector<Polygon*> polygons);
	std::vector<Polygon*> *getPolygons();
	QString getSketch();
	int getItemIndex();
	int size();

	QMatrix4x4 toMatrix();

private:
	Space::Plane *basePlane;
	QVector<Space::Plane*> planes;
	QString sketch;

	//pointers to the global buffer
	std::vector<Vertex*> vertexes;
	//original vertex data
	std::vector<Polygon*> polygons;

	int itemIndex;

signals:
	void sizeChanged();
};

inline QString Item::getSketch() { return sketch; }
inline int Item::getItemIndex() { return itemIndex; }
inline QMatrix4x4 Item::toMatrix(){ return basePlane->toMatrix(); }

#endif // ITEM_H
