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

	SpaceFactory::init(&objectsInSpace, &planes, this);
	factory = SpaceFactory::getInstance();
	connect(factory, &SpaceFactory::reallocateItems,
			this, &View3DWidget::reallocateItems
			);
	connect(factory, &SpaceFactory::allocateNewItem,
			this, &View3DWidget::allocateNewItem
			);
	connect(factory, &SpaceFactory::reallocatePlanes,
			this, &View3DWidget::reallocatePlanes
			);
	connect(factory, &SpaceFactory::allocateNewPlane,
			this, &View3DWidget::allocateNewPlane
			);
}

void View3DWidget::setTopPlane(Plane *plane)
{
	planes.push_back(plane);
	reallocatePlanes();
}

void View3DWidget::loadFromFile(QString fileContents)
{

}

void View3DWidget::saveToFile(QString file)
{
	QFile targetFile(file);
	if(targetFile.open(QIODevice::WriteOnly))
	{
		targetFile.write("[");

		for(int i = 0; i < this->objectsInSpace.length(); i++)
		{
			std::string s = objectsInSpace.at(i)->toJson().dump() + ",";
			targetFile.write(QString::fromStdString(s).toUtf8());
		}

		targetFile.write("]");
	}
}

void View3DWidget::reallocateItems()
{
	vertexData.clear();

	foreach(Item *item, objectsInSpace)
	{
		allocateNewItem(item);
	}
}

void View3DWidget::allocateNewItem(Item *item)
{
	int newItemIndex = vertexData.size();
	vertexData.resize(newItemIndex + item->size());

	std::vector<Vertex*> itemData;
	for(uint32_t i = newItemIndex; i < vertexData.size(); i++)
	{
		itemData.push_back(&vertexData[i]);
	}

	item->copyVertexesToReference(itemData, newItemIndex);

	vertexBuffer.bind();
	vertexBuffer.allocate(vertexData.data(), vertexData.size() * sizeof(Vertex));
	vertexBuffer.release();

	update();
}

void View3DWidget::reallocatePlanes()
{
	planeVertexData.clear();

	for(int i = 0; i < planes.size(); i++ )
	{
		allocateNewPlane();
	}
}

void View3DWidget::allocateNewPlane()
{
	QVector3D planeColor = Settings::planeColor;
	double planeSize = 20;

	planeVertexData.push_back(
				Vertex(QVector3D(planeSize, planeSize, 0), planeColor)
				);
	planeVertexData.push_back(
				Vertex(QVector3D(planeSize, -planeSize, 0), planeColor)
				);
	planeVertexData.push_back(
				Vertex(QVector3D(-planeSize, -planeSize, 0), planeColor)
				);
	planeVertexData.push_back(
				Vertex(QVector3D(planeSize, planeSize, 0), planeColor)
				);
	planeVertexData.push_back(
				Vertex(QVector3D(-planeSize, planeSize, 0), planeColor)
				);
	planeVertexData.push_back(
				Vertex(QVector3D(-planeSize, -planeSize, 0), planeColor)
				);

	planeBuffer.bind();
	planeBuffer.allocate(planeVertexData.data(), planeVertexData.size() * sizeof(Vertex));
	planeBuffer.release();
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

	if(event->buttons() == Qt::RightButton)
	{
		static float rotSpeed = 0.001;

		// Handle rotations
		itemRotation.setX(itemRotation.x() + (lastPos.y() - event->y()));
		itemRotation.setY(itemRotation.y() + (lastPos.x() - event->x()));

		float scalar = std::sqrt((event->x() * event->x()) + (event->y() * event->y()));
		itemRotation.setScalar(itemRotation.scalar() + scalar*rotSpeed);

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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);


	vertexProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/src/shaders/simple.vert");
	vertexProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/src/shaders/simple.frag");
	vertexProgram.link();
	vertexProgram.bind();

	itemToSpace = vertexProgram.uniformLocation("itemToSpace");
	worldToCamera = vertexProgram.uniformLocation("worldToCamera");
	cameraToView = vertexProgram.uniformLocation("cameraToView");
	itemToRotate = vertexProgram.uniformLocation("itemToRotate");

	selectedItemColor = vertexProgram.uniformLocation("selectedItemColor");
	isSelected = vertexProgram.uniformLocation("isSelected");

	transparentColorValue = vertexProgram.uniformLocation("transparentColorValue");
	selectedTransparentValue = vertexProgram.uniformLocation("selectedTransparentValue");

	vertexProgram.release();

	//item buffer

	vertexBuffer.create();
	vertexBuffer.bind();
	vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vertexBuffer.allocate(vertexData.data(), vertexData.size() * sizeof(Vertex));

	vertexBufferObject.create();
	vertexBufferObject.bind();

	vertexProgram.enableAttributeArray(0);
	vertexProgram.enableAttributeArray(1);
	vertexProgram.setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
	vertexProgram.setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

	vertexBufferObject.release();
	vertexBuffer.release();


	//planes buffer

	planeBuffer.create();
	planeBuffer.bind();
	planeBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	planeBuffer.allocate(planeVertexData.data(), planeVertexData.size() * sizeof(Vertex));

	planeBufferObject.create();
	planeBufferObject.bind();

	vertexProgram.enableAttributeArray(0);
	vertexProgram.enableAttributeArray(1);
	vertexProgram.setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
	vertexProgram.setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

	planeBuffer.release();
	planeBufferObject.release();

}

void View3DWidget::resizeGL(int width, int height)
{
	static float near = 0.1;
	static float far = 1000;

	projection.setToIdentity();
	projection.perspective(45.0f, width / float(height), near, far);
}

void View3DWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vertexProgram.bind();
	vertexProgram.setUniformValue(worldToCamera, camera.toMatrix());
	vertexProgram.setUniformValue(cameraToView, projection);
	vertexProgram.setUniformValue(selectedItemColor, Settings::selectedFaceColor);

	QMatrix4x4 mtr;
	mtr.rotate(itemRotation.normalized());
	vertexProgram.setUniformValue(itemToRotate, mtr);

	//items
	vertexBufferObject.bind();
	vertexProgram.setUniformValue(transparentColorValue, 1.0f);
	vertexProgram.setUniformValue(selectedTransparentValue, 1.0f);

	foreach (Item *item, objectsInSpace)
	{
		int currentIndex = item->getItemIndex();
		vertexProgram.setUniformValue(itemToSpace, item->toMatrix());

		if(item->isExtruded())
		{
			vertexProgram.setUniformValue(isSelected, item->isSelected());
			glDrawArrays(GL_TRIANGLES, currentIndex, item->size());
			currentIndex += item->size();
		}
		else
		{
			for(uint32_t i = 0; i < item->getPolygons()->size(); i++)
			{
				vertexProgram.setUniformValue(isSelected, item->isSelected() || item->getPolygons()->at(i)->isSelected());
				glDrawArrays(GL_TRIANGLES, currentIndex, item->getPolygons()->at(i)->size());
				currentIndex += item->getPolygons()->at(i)->size();
			}
		}
	}
	vertexBufferObject.release();

	//planes
	planeBufferObject.bind();
	vertexProgram.setUniformValue(transparentColorValue, 0.0f);
	vertexProgram.setUniformValue(selectedTransparentValue, 0.3f);
	vertexProgram.setUniformValue(selectedItemColor, Settings::planeColor);

	for(int i = 0; i < planes.size(); i++)
	{
		vertexProgram.setUniformValue(isSelected, planes.at(i)->checkState(0) == Qt::Checked);
		vertexProgram.setUniformValue(itemToSpace, planes.at(i)->toMatrix());
		glDrawArrays(GL_TRIANGLES, i * 6, 6);
	}

	planeBufferObject.release();

	vertexProgram.release();
}
