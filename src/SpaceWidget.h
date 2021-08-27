#ifndef SPACEWIDGET_H
#define SPACEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QFrame>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include "Dialogs/MessagesManager.h"
#include "Space/Space.h"

struct vertexProgramParameters{
	int itemToSpace, worldToCamera, cameraToView, itemToRotate;
};

class SpaceWidget : public QOpenGLWidget, private QOpenGLFunctions
{
	Q_OBJECT
public:
	SpaceWidget(QWidget *parent);

	void setTopPlane(Plane *plane);

	//file operations
	void loadFromFile(QString fileContents);
	QString saveToFile();
	void exportToFile(QString file);

	void reset();

public slots:
	void reallocateItems();
	void reallocatePlanes();
	void update();

	Plane *getBasePlane();

private:
	QQuaternion itemRotation;
	Camera camera;
	QMatrix4x4 projection;
	SpaceFactory *factory;

	QVector<Item*> objectsInSpace;
	QOpenGLShaderProgram itemProgram;
	QOpenGLShaderProgram planesProgram;
	QOpenGLShaderProgram linesProgram;

	//processed vertexes data
	std::vector<Vertex> itemVertexData;
	QOpenGLBuffer itemBuffer;
	QOpenGLVertexArrayObject itemBufferObject;

	//processed plane data
	QVector<Plane*> planes;
	std::vector<Vertex> planeVertexData;
	QOpenGLBuffer planeBuffer;
	QOpenGLVertexArrayObject planeBufferObject;

	//processed line data
	std::vector<Vertex> lineVertexData;
	QOpenGLBuffer lineBuffer;
	QOpenGLVertexArrayObject lineBufferObject;

	// Shader Information
	vertexProgramParameters itemParameter;
	vertexProgramParameters planesParameter;
	vertexProgramParameters linesParameter;

	int itemColor;
	int planeColor;
	int lineColor;

	int selectedItemColor;
	int itemIsSelected;

	int selectedTransparentValue;
	int lightByNormals;

	QPoint lastPos;

	//events
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;
};

inline void SpaceWidget::update() { QOpenGLWidget::update(); }

#endif // SPACEWIDGET_H
