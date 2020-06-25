#ifndef ITEM_H
#define ITEM_H

#include "Vertex.h"
#include "Transform3D.h"

#include "IntermediateObjects/TransferLine.h"

//triangle
#define VERTEX_0 Vertex( QVector3D( 0.0f,  0.0f,  -5.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_1 Vertex( QVector3D( -0.5f,  0.0f,  -4.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_2 Vertex( QVector3D( 0.5f,  0.0f,  -4.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_TOP Vertex( QVector3D( 0.0f,  1.0f,  -5.0f), QVector3D( 1.0f, 1.0f, 1.0f ) )


class Item
{
public:
	Item(QVector<DrawableObject*> sketchObjects, QVector3D planePosition, QVector3D planeVector);

	Transform3D *getTransform();

	void setVectorReference(std::vector<Vertex*> vector, int itemIndex);

	int getItemIndex();
	int size();

private:
	Transform3D transform;

	//pointers to the global buffer
	std::vector<Vertex*> vertexes;
	//original vertex data
	std::vector<Vertex> vertexData;

	QVector<DrawableObject*> sketchObjects;
	QVector3D planePosition;
	QVector3D planeVector;

	int itemIndex;

	void generateVertexes(QVector3D planePosition, QVector3D planeVector);
		void convertToIntermediate(QVector<TransferPoint*> *transferPoints, QVector<TransferLine*> *transferLines);
		void assignNeigbors(QVector<TransferPoint*> *transferPoints, QVector<TransferLine*> *transferLines);
		std::vector<Vertex> triangularize(QVector<TransferPoint*> *transferPoints);
		std::vector<Vertex> pointsToSpaceVertexes(std::vector<Vertex> planeVertexes, QVector3D planePosition, QVector3D planeVector);
};

inline Transform3D *Item::getTransform() { return &transform; }
inline int Item::getItemIndex() { return itemIndex; }
inline int Item::size() { return vertexData.size(); }

#endif // ITEM_H
