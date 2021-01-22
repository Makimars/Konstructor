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

	connect(factory, &SpaceFactory::getBasePlane,
			this, &View3DWidget::getBasePlane
			);
}

void View3DWidget::setTopPlane(Plane *plane)
{
	planes.push_back(plane);
	reallocatePlanes();
}

void View3DWidget::loadFromFile(QString fileContents)
{
	nlohmann::json input = nlohmann::json::parse(fileContents.toStdString());

	std::vector<std::string> objects = input["objects"];
	std::vector<Item*> items;

	for (uint32_t i = 0; i < objects.size(); i++)
	{
		//load static data
		items.push_back(factory->loadItem(objects.at(i)));
	}

	for (uint32_t i = 0; i < items.size(); i++)
	{
		//link base planes
		items.at(i)->loadRelations(items);
		if(items.at(i)->isExtruded())
		{
			items.at(i)->extrude();
		}
	}
	for (uint32_t i = 0; i < items.size(); i++)
	{
		factory->addItem(items.at(i));
	}
}

void View3DWidget::saveToFile(QString file)
{
	std::vector<std::string> objects;
	for(int i = 0; i < this->objectsInSpace.length(); i++)
	{
		std::string s = objectsInSpace.at(i)->toJson().dump();
		objects.push_back(s);
	}

	QFile targetFile(file);
	if(targetFile.open(QIODevice::WriteOnly))
	{
		nlohmann::json json;
		json["objects"] = objects;
		targetFile.write(QString::fromStdString(std::string(json.dump())).toUtf8());
	}
}

void View3DWidget::reset()
{
	Plane *basePlane = planes.at(0);
	planes.clear();
	setTopPlane(basePlane);

	factory->deleteAllItems();
	update();
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
	item->setItemIndex(vertexData.size());

	factory->generateItemVertexDataToBuffer(item, &vertexData);

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
	double planeSize = 20;

	planeVertexData.push_back(
				Vertex(planeSize, planeSize, 0)
				);
	planeVertexData.push_back(
				Vertex(planeSize, -planeSize, 0)
				);
	planeVertexData.push_back(
				Vertex(-planeSize, -planeSize, 0)
				);
	planeVertexData.push_back(
				Vertex(planeSize, planeSize, 0)
				);
	planeVertexData.push_back(
				Vertex(-planeSize, planeSize, 0)
				);
	planeVertexData.push_back(
				Vertex(-planeSize, -planeSize, 0)
				);

	planeBuffer.bind();
	planeBuffer.allocate(planeVertexData.data(), planeVertexData.size() * sizeof(Vertex));
	planeBuffer.release();
}

Plane *View3DWidget::getBasePlane()
{
	return planes.at(0);
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

	//item buffer

	vertexProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/src/shaders/simple.vert");
	vertexProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/src/shaders/simple.frag");
	vertexProgram.link();
	vertexProgram.bind();

	vertexParameter.itemToSpace = vertexProgram.uniformLocation("itemToSpace");
	vertexParameter.worldToCamera = vertexProgram.uniformLocation("worldToCamera");
	vertexParameter.cameraToView = vertexProgram.uniformLocation("cameraToView");
	vertexParameter.itemToRotate = vertexProgram.uniformLocation("itemToRotate");

	selectedItemColor = vertexProgram.uniformLocation("selectedItemColor");
	itemIsSelected = vertexProgram.uniformLocation("isSelected");

	itemColor = vertexProgram.uniformLocation("itemColor");
	selectedTransparentValue = vertexProgram.uniformLocation("selectedTransparentValue");

	vertexProgram.release();

	vertexBuffer.create();
	vertexBuffer.bind();
	vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vertexBuffer.allocate(vertexData.data(), vertexData.size() * sizeof(Vertex));

	vertexBufferObject.create();
	vertexBufferObject.bind();

	vertexProgram.enableAttributeArray(0);
	vertexProgram.enableAttributeArray(1);
	vertexProgram.setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
	vertexBufferObject.release();
	vertexBuffer.release();


	//planes buffer

	planesProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/src/shaders/simple.vert");
	planesProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/src/shaders/planes.frag");
	planesProgram.link();
	planesProgram.bind();

	planesParameter.itemToSpace = planesProgram.uniformLocation("itemToSpace");
	planesParameter.worldToCamera = planesProgram.uniformLocation("worldToCamera");
	planesParameter.cameraToView = planesProgram.uniformLocation("cameraToView");
	planesParameter.itemToRotate = planesProgram.uniformLocation("itemToRotate");
	planeColor = planesProgram.uniformLocation("planeColor");

	planeIsSelected = planesProgram.uniformLocation("isSelected");

	planesProgram.release();


	planeBuffer.create();
	planeBuffer.bind();
	planeBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	planeBuffer.allocate(planeVertexData.data(), planeVertexData.size() * sizeof(Vertex));

	planeBufferObject.create();
	planeBufferObject.bind();

	planesProgram.enableAttributeArray(0);
	planesProgram.enableAttributeArray(1);
	planesProgram.setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());

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
	vertexProgram.setUniformValue(vertexParameter.worldToCamera, camera.toMatrix());
	vertexProgram.setUniformValue(vertexParameter.cameraToView, projection);
	vertexProgram.setUniformValue(selectedItemColor, Settings::selectedFaceColor);

	QMatrix4x4 mtr;
	mtr.rotate(itemRotation.normalized());
	vertexProgram.setUniformValue(vertexParameter.itemToRotate, mtr);

	//items
	vertexBufferObject.bind();
	vertexProgram.setUniformValue(selectedTransparentValue, 1.0f);

	foreach (Item *item, objectsInSpace)
	{
		int currentIndex = item->getItemIndex();
		vertexProgram.setUniformValue(vertexParameter.itemToSpace, item->toMatrix());

		if(item->isExtruded())
		{
			vertexProgram.setUniformValue(itemColor, QVector4D(item->getColor(), 1.0f));
			vertexProgram.setUniformValue(itemIsSelected, item->isSelected());
			glDrawArrays(GL_TRIANGLES, currentIndex, item->getDataSize());
		}
		else
		{
			for(int i = 0; i < item->getPolygons()->size(); i++)
			{
				vertexProgram.setUniformValue(itemColor, QVector4D(item->getPolygons()->at(i)->getColor(), 1.0f));
				vertexProgram.setUniformValue(itemIsSelected, item->isSelected() || item->getPolygons()->at(i)->isSelected());
				glDrawArrays(GL_TRIANGLES, currentIndex, item->getPolygons()->at(i)->getDataSize());
				currentIndex += item->getPolygons()->at(i)->getDataSize();
			}
		}
	}
	vertexBufferObject.release();

	vertexProgram.release();

	//planes
	planesProgram.bind();
	planeBufferObject.bind();

	planesProgram.setUniformValue(planesParameter.worldToCamera, camera.toMatrix());
	planesProgram.setUniformValue(planesParameter.cameraToView, projection);
	planesProgram.setUniformValue(planeColor, Settings::planeColor);

	vertexProgram.setUniformValue(vertexParameter.itemToRotate, mtr);

	for(int i = 0; i < planes.size(); i++)
	{
		planesProgram.setUniformValue(planeIsSelected, planes.at(i)->checkState(0) == Qt::Checked);
		planesProgram.setUniformValue(planesParameter.itemToSpace, planes.at(i)->toMatrix());
		glDrawArrays(GL_TRIANGLES, i * 6, 6);
	}

	planeBufferObject.release();

	planesProgram.release();
}
