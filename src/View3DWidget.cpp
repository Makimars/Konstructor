#include "View3DWidget.h"

View3DWidget::View3DWidget(QFrame *frame) : QOpenGLWidget(frame)
{
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3,3);
	setFormat(format);

	setFocus();
}

void View3DWidget::mousePressEvent(QMouseEvent *event)
{
	QOpenGLWidget::mousePressEvent(event);
}

void View3DWidget::mouseReleaseEvent(QMouseEvent *event)
{
	QOpenGLWidget::mouseReleaseEvent(event);

	if(this->selectedTool != nullptr)
		this->selectedTool->click(event->pos());
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

void View3DWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/src/shaders/simple.vert");
	program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/src/shaders/simple.frag");
	program.link();
	program.bind();

	modelToWorld = program.uniformLocation("modelToWorld");
	worldToCamera = program.uniformLocation("worldToCamera");
	cameraToView = program.uniformLocation("cameraToView");

	foreach (Mesh *m, objects)
	{
		m->initializeGl(&program, modelToWorld);
	}

	program.release();
}

void View3DWidget::resizeGL(int width, int height)
{
	projection.setToIdentity();
	projection.perspective(45.0f, width / float(height), 0.0f, 1000.0f);
}

void View3DWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.bind();
	program.setUniformValue(worldToCamera, 1/*Camera.toMatrix*/);
	program.setUniformValue(cameraToView, projection);

	foreach (Mesh *m, objects)
	{
		m->render(&program);
	}

	program.release();
}

void View3DWidget::setTool(int tool)
{
	if(this->selectedTool != nullptr)
		this->selectedTool->resetTool();
	switch(tool)
	{
		case Global::Tools::Draw:
			this->selectedTool = DrawTool::getInstance();
			break;
		default:
			this->selectedTool = nullptr;
			break;
	}
}

void View3DWidget::resetTool()
{
	if(this->selectedTool != nullptr)
		this->selectedTool->resetTool();
}

void View3DWidget::addItem(Item *item)
{
	objectsInSpace.append(item);
}
