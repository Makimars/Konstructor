#ifndef VIEW3DWIDGET_H
#define VIEW3DWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QFrame>

#include "ViewWidget.h"
#include "ClassRepository/3D/Tools/DrawTool.h"
#include "ClassRepository/3D/Mesh.h"

class View3DWidget : public QOpenGLWidget, private QOpenGLFunctions
{
public:
		View3DWidget(QFrame *frame);

private:
	QList<Mesh*> objects;
	QMatrix4x4 projection;
	QOpenGLShaderProgram program;

	// Shader Information
	int modelToWorld;
	int worldToCamera;
	int cameraToView;

	//tools
	Tool3D *selectedTool;

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
	void setTool(QString toolName);
	void resetTool();
};

#endif // VIEW3DWIDGET_H
