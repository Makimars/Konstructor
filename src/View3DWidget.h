#ifndef VIEW3DWIDGET_H
#define VIEW3DWIDGET_H

#include <QOpenGLWidget>
#include <QFrame>

#include "ViewWidget.h"
#include "ClassRepository/Tools3D/DrawTool.h"

class View3DWidget : public QOpenGLWidget
{
public:
		View3DWidget(QFrame *frame);

private:

	//tools
	Tool3D *selectedTool;

	//events
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

public slots:
	void setTool(QString toolName);
	void resetTool();
};

#endif // VIEW3DWIDGET_H
