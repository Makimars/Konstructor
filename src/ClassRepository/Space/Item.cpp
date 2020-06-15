#include "Item.h"

//create coloured triangle
std::vector<Vertex> triangles_vertexes = {
	VERTEX_1, VERTEX_2, VERTEX_TOP,
	VERTEX_2, VERTEX_0, VERTEX_TOP,
	VERTEX_0, VERTEX_1, VERTEX_TOP,
	VERTEX_2, VERTEX_1, VERTEX_0,
};

Item::Item(QVector<DrawableObject*> sketchObjects, QPointF planePosition, QVector3D planeVector)
{
	this->sketchObjects = sketchObjects;
	generateVertexes();
}

void Item::setVector(std::vector<Vertex*> vector, int itemIndex)
{
	vertexes = vector;
	this->itemIndex = itemIndex;

	for(int i = 0; i < vertexData.size(); i++)
		*vertexes[i] = vertexData[i];
}

void Item::generateVertexes()
{
	vertexData = triangles_vertexes;


	vertexes.reserve(size());
}
