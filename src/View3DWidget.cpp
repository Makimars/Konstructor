#include "View3DWidget.h"

View3DWidget::View3DWidget(QFrame *frame) : QOpenGLWidget(frame)
{

}

void View3DWidget::mousePressEvent(QMouseEvent *event)
{
	QOpenGLWidget::mousePressEvent(event);
}

void View3DWidget::mouseReleaseEvent(QMouseEvent *event)
{
	QOpenGLWidget::mouseReleaseEvent(event);
}

void View3DWidget::mouseMoveEvent(QMouseEvent *event)
{
	QOpenGLWidget::mouseMoveEvent(event);

}

void View3DWidget::wheelEvent(QWheelEvent *event)
{
	QOpenGLWidget::wheelEvent(event);

}

void View3DWidget::keyPressEvent(QKeyEvent *event)
{
	QOpenGLWidget::keyPressEvent(event);

}
