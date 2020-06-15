#include "View3DWidget.h"

View3DWidget::View3DWidget(QFrame *frame) : QOpenGLWidget(frame)
{
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3,3);
	setFormat(format);

	connect(DrawTool::getInstance(), &DrawTool::addItem,
			this, &View3DWidget::addItem
			);

	//test
	Item *item = new Item(QVector<DrawableObject*>(), QPointF(), QVector3D());
	addItem(item);

	setFocus();
}

void View3DWidget::mousePressEvent(QMouseEvent *event)
{
	QOpenGLWidget::mousePressEvent(event);

	//test
	Item *item2 = new Item(QVector<DrawableObject*>(), QPointF(), QVector3D());
	item2->getTransform()->translate(0,1,1);
	addItem(item2);
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

	static const float rotSpeed   = 0.5f;

	if(event->button() == Qt::RightButton)
	{
		// Handle rotations
		camera.rotate(-rotSpeed * event->x(), Camera::LocalUp);
		camera.rotate(-rotSpeed * event->y(), camera.right());

		// redraw
		QOpenGLWidget::update();
	}
}

void View3DWidget::wheelEvent(QWheelEvent *event)
{
	QOpenGLWidget::wheelEvent(event);
}

void View3DWidget::keyPressEvent(QKeyEvent *event)
{
	QOpenGLWidget::keyPressEvent(event);

	static const float transSpeed = 0.5f;

	// Handle translations
	QVector3D translation;
	if (event->key() == Qt::Key_W)
	{
	  translation += camera.forward();
	}
	if (event->key() == Qt::Key_S)
	{
	  translation -= camera.forward();
	}
	if (event->key() == Qt::Key_A)
	{
	  translation -= camera.right();
	}
	if (event->key() == Qt::Key_D)
	{
	  translation += camera.right();
	}
	if (event->key() == Qt::Key_Q)
	{
	  translation -= camera.up();
	}
	if (event->key() == Qt::Key_E)
	{
	  translation += camera.up();
	}
	camera.translate(transSpeed * translation);

	// redraw
	QOpenGLWidget::update();
}

void View3DWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glEnable(GL_CULL_FACE);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/src/shaders/simple.vert");
	program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/src/shaders/simple.frag");
	program.link();
	program.bind();

	modelToWorld = program.uniformLocation("modelToWorld");
	worldToCamera = program.uniformLocation("worldToCamera");
	cameraToView = program.uniformLocation("cameraToView");

	vertexBuffer.create();
	vertexBuffer.bind();
	vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vertexBuffer.allocate(vertexData.data(), vertexData.size() * sizeof(Vertex));

	vertexBufferObject.create();
	vertexBufferObject.bind();

	program.enableAttributeArray(0);
	program.enableAttributeArray(1);
	program.setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
	program.setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

	vertexBufferObject.release();
	vertexBuffer.release();

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
	program.setUniformValue(worldToCamera, camera.toMatrix());
	program.setUniformValue(cameraToView, projection);

	vertexBufferObject.bind();
	foreach (Item *item, objectsInSpace)
	{
		program.setUniformValue(modelToWorld, item->getTransform()->toMatrix());
		glDrawArrays(GL_TRIANGLES, item->getItemIndex(), item->size());
	}
	vertexBufferObject.release();

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

	int newItemIndex = vertexData.size();
	vertexData.resize(newItemIndex + item->size());

	std::vector<Vertex*> itemData;
	for(int i = newItemIndex; i < vertexData.size(); i++)
	{
		itemData.push_back(&vertexData[i]);
	}

	item->setVector(itemData, newItemIndex);

	vertexBuffer.bind();
	vertexBuffer.allocate(vertexData.data(), vertexData.size() * sizeof(Vertex));
	vertexBuffer.release();

	update();
}
