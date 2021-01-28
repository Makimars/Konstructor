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

#include "Dialogs/MessagesManager.h"
#include "Space/Space"

struct vertexProgramParameters{
	int itemToSpace, worldToCamera, cameraToView, itemToRotate;
};

class View3DWidget : public QOpenGLWidget, private QOpenGLFunctions
{
	Q_OBJECT
public:
	View3DWidget(QWidget *parent);

	void setTopPlane(Plane *plane);

	//file operations
	void loadFromFile(QString fileContents);
	void saveToFile(QString file);
	void exportToFile(QString file);

	void reset();

public slots:
	void reallocateItems();
	void allocateNewItem(Item *item);
	void reallocatePlanes();
	void allocateNewPlane();
	void update();

	Plane *getBasePlane();

private:
	QQuaternion itemRotation;
	Camera camera;
	QMatrix4x4 projection;
	SpaceFactory *factory;

	QVector<Item*> objectsInSpace;
	QOpenGLShaderProgram vertexProgram;
	QOpenGLShaderProgram planesProgram;

	//processed vertexes data
	std::vector<Vertex> vertexData;
	QOpenGLBuffer vertexBuffer;
	QOpenGLVertexArrayObject vertexBufferObject;

	//processed plane data
	QVector<Plane*> planes;
	std::vector<Vertex> planeVertexData;
	QOpenGLBuffer planeBuffer;
	QOpenGLVertexArrayObject planeBufferObject;


	// Shader Information
	vertexProgramParameters vertexParameter;
	vertexProgramParameters planesParameter;

	int itemColor;
	int planeColor;

	int selectedItemColor;
	int itemIsSelected;
	int planeIsSelected;

	int selectedTransparentValue;

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

inline void View3DWidget::update() { QOpenGLWidget::update(); }

#endif // VIEW3DWIDGET_H
