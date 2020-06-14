#include "Item.h"

//triangle
#define VERTEX_0 Vertex( QVector3D( 0.0f,  0.0f,  -0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_1 Vertex( QVector3D( -0.5f,  0.0f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_2 Vertex( QVector3D( 0.5f,  0.0f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_TOP Vertex( QVector3D( 0.0f,  1.0f,  0.0f), QVector3D( 1.0f, 1.0f, 1.0f ) )


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

void Item::generateVertexes()
{
	mesh.setVertexes(triangles_vertexes);
}
