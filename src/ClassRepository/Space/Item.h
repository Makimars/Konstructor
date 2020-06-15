#ifndef ITEM_H
#define ITEM_H

#include "Vertex.h"
#include "Transform3D.h"

#include "src/ClassRepository/Plane/Drawables/Dimensions/CirclesRadiusDifferenceDimension.h"

//triangle
#define VERTEX_0 Vertex( QVector3D( 0.0f,  0.0f,  -5.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_1 Vertex( QVector3D( -0.5f,  0.0f,  -4.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_2 Vertex( QVector3D( 0.5f,  0.0f,  -4.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_TOP Vertex( QVector3D( 0.0f,  1.0f,  -5.0f), QVector3D( 1.0f, 1.0f, 1.0f ) )


class Item
{
public:
	Item(QVector<DrawableObject*> sketchObjects, QPointF planePosition, QVector3D planeVector);

	Transform3D *getTransform();

	void setVector(std::vector<Vertex*> vector, int itemIndex);

	int getItemIndex();
	int size();

private:
	Transform3D transform;

	std::vector<Vertex*> vertexes;
	std::vector<Vertex> vertexData;

	QVector<DrawableObject*> sketchObjects;
	QPointF planePosition;
	QVector3D planeVector;

	int itemIndex;

	void generateVertexes();
};

inline Transform3D *Item::getTransform() { return &transform; }
inline int Item::getItemIndex() { return itemIndex; }
inline int Item::size() { return vertexData.size(); }

#endif // ITEM_H
