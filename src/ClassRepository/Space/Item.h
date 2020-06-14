#ifndef ITEM_H
#define ITEM_H

#include "Mesh.h"
#include "Transform3D.h"

#include "src/ClassRepository/Plane/Drawables/Dimensions/CirclesRadiusDifferenceDimension.h"



//triangle
#define VERTEX_0 Vertex( QVector3D( 0.0f,  0.0f,  -0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_1 Vertex( QVector3D( -0.5f,  0.0f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_2 Vertex( QVector3D( 0.5f,  0.0f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_TOP Vertex( QVector3D( 0.0f,  1.0f,  0.0f), QVector3D( 1.0f, 1.0f, 1.0f ) )


class Item
{
public:
	Item(QVector<DrawableObject*> sketchObjects, QPointF planePosition, QVector3D planeVector);

	void initializeGl(QOpenGLShaderProgram *program,int modelToWroldAddress);
	void render(QOpenGLShaderProgram *program);
	Transform3D *getTransform();

private:
	Mesh mesh;
	QVector<DrawableObject*> sketchObjects;
	QPointF planePosition;
	QVector3D planeVector;

	void generateVertexes();
};

inline void Item::initializeGl(QOpenGLShaderProgram *program,int modelToWroldAddress) { mesh.initializeGl(program, modelToWroldAddress); }
inline void Item::render(QOpenGLShaderProgram *program) { mesh.render(program); }
inline Transform3D *Item::getTransform() { return  mesh.getTransform(); }

#endif // ITEM_H
