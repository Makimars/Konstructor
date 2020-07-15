#ifndef ITEM_H
#define ITEM_H

#include "Vertex.h"
#include "Transform3D.h"
#include "Plane.h"
#include "include/delaunator/delaunator.h"

#include "IntermediateObjects/TransferLine.h"

class Item : public QTreeWidgetItem
{
public:
	Item(QVector<DrawableObject*> sketchObjects, Space::Plane *plane);

	Transform3D *getTransform();
	void setVectorReference(std::vector<Vertex*> vector, int itemIndex);

	int getItemIndex();
	int size();

private:
	QString name;
	Space::Plane *basePlane;
	QVector<Space::Plane*> planes;

	Transform3D transform;

	//pointers to the global buffer
	std::vector<Vertex*> vertexes;
	//original vertex data
	std::vector<Vertex> vertexData;

	QVector<DrawableObject*> sketchObjects;

	int itemIndex;

	void generateVertexes(Space::Plane *plane);
		void convertToIntermediate(QVector<TransferPoint*> *transferPoints, QVector<TransferLine*> *transferLines);
		void assignNeigbors(QVector<TransferPoint*> *transferPoints, QVector<TransferLine*> *transferLines);
		std::vector<Vertex> triangularize(QVector<TransferPoint*> *transferPoints);
		std::vector<Vertex> pointsToSpaceVertexes(std::vector<Vertex> planeVertexes, Space::Plane *plane);
};

inline Transform3D *Item::getTransform() { return &transform; }
inline int Item::getItemIndex() { return itemIndex; }
inline int Item::size() { return vertexData.size(); }

#endif // ITEM_H
