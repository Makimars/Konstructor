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

#include "ClassRepository/Space/Space"
#include "MessagesManager.h"

class View3DWidget : public QOpenGLWidget, private QOpenGLFunctions
{
	Q_OBJECT
public:
	View3DWidget(QFrame *frame);

private:
	QVector<Item*> objectsInSpace;
	QMatrix4x4 projection;
	QOpenGLShaderProgram program;
	Camera camera;

	//copy of all vertexes data
	std::vector<Vertex> vertexData;
	QOpenGLBuffer vertexBuffer;
	QOpenGLVertexArrayObject vertexBufferObject;

	QTreeWidgetItem *targetItem;

	// Shader Information
	int itemToSpace;
	int worldToCamera;
	int cameraToView;

	int selectedItemColor;
	int itemIsSelected;

	QPoint lastPos;

	void allocateNewItem(Item *item);

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
	void addItem(std::vector<QPolygonF> polygons, QString sketch);
	void recieveTargetItem(QTreeWidgetItem *item);
	void reallocateMemory();
};

#endif // VIEW3DWIDGET_H
