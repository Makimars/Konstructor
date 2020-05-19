#include "Mesh.h"

Mesh::Mesh()
{

}

void Mesh::initializeGl(QOpenGLShaderProgram *program)
{
	initializeOpenGLFunctions();

	vertexBuffer.create();
	vertexBuffer.bind();
	vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vertexBuffer.allocate(vertexes.data(), vertexes.size() * sizeof(Vertex));

	vertexBufferObject.create();
	vertexBufferObject.bind();

	program->enableAttributeArray(0);
	program->enableAttributeArray(1);
	program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
	program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

	vertexBufferObject.release();
	vertexBuffer.release();
}

void Mesh::render(int modelToWroldAddress, QOpenGLShaderProgram *program)
{
	vertexBufferObject.bind();

	program->setUniformValue(modelToWroldAddress, transform.toMatrix());
	glDrawArrays(GL_TRIANGLES, 0, vertexes.size());

	vertexBufferObject.release();
}
