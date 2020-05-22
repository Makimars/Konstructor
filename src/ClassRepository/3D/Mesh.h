#ifndef MESH_H
#define MESH_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>


#include "Vertex.h"
#include "Transform3D.h"

class Mesh : private QOpenGLFunctions
{
public:
	Mesh();

	void initializeGl(QOpenGLShaderProgram *program,int modelToWroldAddress);
	void render(QOpenGLShaderProgram *program);

	void setVertexes(std::vector<Vertex> vertexes);
	Transform3D *getTransform();

private:
	std::vector<Vertex> vertexes;
	int modelToWrold;

	QOpenGLBuffer vertexBuffer;
	QOpenGLVertexArrayObject vertexBufferObject;
	Transform3D transform;
};

inline void Mesh::setVertexes(std::vector<Vertex> vertexes) { this->vertexes = vertexes; }
inline Transform3D *Mesh::getTransform(){ return &transform; }

#endif // MESH_H
