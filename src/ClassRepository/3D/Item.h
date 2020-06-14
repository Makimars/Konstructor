#ifndef ITEM_H
#define ITEM_H

#include "Mesh.h"
#include "Transform3D.h"

#include "src/ClassRepository/2D/Drawables/Dimensions/CirclesRadiusDifferenceDimension.h"

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
