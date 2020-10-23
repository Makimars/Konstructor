#include "View3DWidget.h"

View3DWidget::View3DWidget(QFrame *frame) : QOpenGLWidget(frame)
{
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3,3);
	setFormat(format);

	camera.translate(-camera.forward()*Settings::planeToSpaceRatio);
	itemRotation.setScalar(1);

	setFocus();
}

void View3DWidget::allocateNewItem(Item *item)
{
	int newItemIndex = vertexData.size();
	vertexData.resize(newItemIndex + item->size());

	std::vector<Vertex*> itemData;
	for(int i = newItemIndex; i < vertexData.size(); i++)
	{
		itemData.push_back(&vertexData[i]);
	}

	item->setVectorReference(itemData, newItemIndex);

	vertexBuffer.bind();
	vertexBuffer.allocate(vertexData.data(), vertexData.size() * sizeof(Vertex));
	vertexBuffer.release();

	update();
}

void View3DWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	QOpenGLWidget::mousePressEvent(event);
}

void View3DWidget::mouseReleaseEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	QOpenGLWidget::mouseReleaseEvent(event);
}

void View3DWidget::mouseMoveEvent(QMouseEvent *event)
{
	QOpenGLWidget::mouseMoveEvent(event);

	static const float rotSpeed   = 0.01f;

	if(event->buttons() == Qt::RightButton)
	{
		// Handle rotations
		itemRotation.setX(itemRotation.x() + (lastPos.y() - event->y()));
		itemRotation.setY(itemRotation.y() + (lastPos.x() - event->x()));

		float scalar = std::sqrt((event->x() * event->x()) + (event->y() * event->y())) * 0.0001;
		itemRotation.setScalar(itemRotation.scalar() + scalar);

		// redraw
		QOpenGLWidget::update();
	}

	lastPos = event->pos();
}

void View3DWidget::wheelEvent(QWheelEvent *event)
{
	QOpenGLWidget::wheelEvent(event);

	QVector3D translation = camera.forward() * event->delta();
	static const float transSpeed = 0.05f;
	camera.translate(transSpeed * translation);

	// redraw
	QOpenGLWidget::update();
}

void View3DWidget::keyPressEvent(QKeyEvent *event)
{
	QOpenGLWidget::keyPressEvent(event);
}

void View3DWidget::initializeGL()
{
	initializeOpenGLFunctions();
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/src/shaders/simple.vert");
	program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/src/shaders/simple.frag");
	program.link();
	program.bind();

	itemToSpace = program.uniformLocation("itemToSpace");
	worldToCamera = program.uniformLocation("worldToCamera");
	cameraToView = program.uniformLocation("cameraToView");
	itemToRotate = program.uniformLocation("itemToRotate");

	selectedItemColor = program.uniformLocation("selectedItemColor");
	itemIsSelected = program.uniformLocation("itemIsSelected");
	polygonIsSelected = program.uniformLocation("polygonIsSelected");

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
	program.setUniformValue(selectedItemColor, Settings::selectedFaceColor);

	QMatrix4x4 mtr;
	mtr.rotate(itemRotation.normalized());
	program.setUniformValue(itemToRotate, mtr);

	vertexBufferObject.bind();
	//glClear(GL_DEPTH_BUFFER_BIT);
	foreach (Item *item, objectsInSpace)
	{
		int currentIndex = item->getItemIndex();
		program.setUniformValue(itemToSpace, item->toMatrix());
		program.setUniformValue(itemIsSelected, item->isSelected());

		for(uint32_t i = 0; i < item->getPolygons()->size(); i++)
		{
			program.setUniformValue(polygonIsSelected, item->getPolygons()->at(i)->isSelected());
			glDrawArrays(GL_TRIANGLES, currentIndex, item->getPolygons()->at(i)->size());
			currentIndex += item->getPolygons()->at(i)->size();
		}
	}
	vertexBufferObject.release();

	program.release();
}

void View3DWidget::addItem(std::vector<QPolygonF> polygons, QString sketch)
{
	if(Space::Plane *plane = dynamic_cast<Space::Plane*>(targetItem))
	{
		Item *item = new Item(plane, polygons, sketch);

		item->setText(0, "object " + QString::number(objectsInSpace.size()));

		objectsInSpace.append(item);
		connect(item, &Item::updateData,
				this, &View3DWidget::reallocateMemory
				);

		allocateNewItem(item);
	}
	else if(Item *item = dynamic_cast<Item*>(targetItem))
	{
		item->setPolygons(polygons);
	}
}

void View3DWidget::recieveTargetItem(QTreeWidgetItem *item)
{
	targetItem = item;
}

void View3DWidget::reallocateMemory()
{
	vertexData.clear();

	foreach(Item *item, objectsInSpace)
	{
		allocateNewItem(item);
	}
}
