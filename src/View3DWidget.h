#ifndef VIEW3DWIDGET_H
#define VIEW3DWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QFrame>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "ClassRepository/Space/Tools/DrawTool.h"
#include "ClassRepository/Space/Camera.h"

class View3DWidget : public QOpenGLWidget, private QOpenGLFunctions
{
public:
	View3DWidget(QFrame *frame);

private:
	QVector<Item*> objectsInSpace;
	QMatrix4x4 projection;
	QOpenGLShaderProgram program;
	Camera camera;

	std::vector<Vertex> vertexData;
	QOpenGLBuffer vertexBuffer;
	QOpenGLVertexArrayObject vertexBufferObject;

	// Shader Information
	int modelToWorld;
	int worldToCamera;
	int cameraToView;

	//tools
	Space::Tool *selectedTool = nullptr;

	//events
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

public slots:
	void setTool(int tool);
	void resetTool();
	void addItem(Item *item);
};

#endif // VIEW3DWIDGET_H
