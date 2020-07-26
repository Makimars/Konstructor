#ifndef ITEM_H
#define ITEM_H

#include <QDebug>

#include "Plane.h"
#include "Vertex.h"

enum ExtrusionDirection
{
    Front,
    FrontAndBack,
    Back
};

class Item : public QObject, public QTreeWidgetItem
{
	Q_OBJECT
public:
	Item(Space::Plane *plane, QString sketch);

	void setVectorReference(std::vector<Vertex*> vector, int itemIndex);
	void setPlaneVertexes(std::vector<Vertex> vertexes);
	QString getSketch();
	int getItemIndex();
	int size();

    void extrude(double length, bool extrusion, ExtrusionDirection direction);

private:
	Space::Plane *basePlane;
	QVector<Space::Plane*> planes;
	QString sketch;

	//pointers to the global buffer
	std::vector<Vertex*> vertexes;
	//original vertex data
	std::vector<Vertex> vertexData;

	int itemIndex;

	std::vector<Vertex> pointsToSpaceVertexes(std::vector<Vertex> planeVertexes);

signals:
	void sizeChanged();
};

inline QString Item::getSketch() { return sketch; }
inline int Item::getItemIndex() { return itemIndex; }
inline int Item::size() { return vertexData.size(); }

#endif // ITEM_H
