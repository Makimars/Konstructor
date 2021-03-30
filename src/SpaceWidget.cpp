#include "SpaceWidget.h"

SpaceWidget::SpaceWidget(QWidget *parent) : QOpenGLWidget(parent)
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
			this, &SpaceWidget::reallocateItems
			);
	connect(factory, &SpaceFactory::reallocatePlanes,
			this, &SpaceWidget::reallocatePlanes
			);
	connect(factory, &SpaceFactory::allocateNewPlane,
			this, &SpaceWidget::allocateNewPlane
			);

	connect(factory, &SpaceFactory::getBasePlane,
			this, &SpaceWidget::getBasePlane
			);
}

void SpaceWidget::setTopPlane(Plane *plane)
{
	planes.push_back(plane);
	reallocatePlanes();
}

void SpaceWidget::loadFromFile(QString fileContents)
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
			items.at(i)->setExtrusion();
		}
	}
	for (uint32_t i = 0; i < items.size(); i++)
	{
		objectsInSpace.append(items.at(i));
	}

	reallocateItems();
}

QString SpaceWidget::saveToFile()
{
	std::vector<std::string> objects;
	for(int i = 0; i < this->objectsInSpace.length(); i++)
	{
		std::string s = objectsInSpace.at(i)->toJson().dump();
		objects.push_back(s);
	}

	nlohmann::json json;
	json["objects"] = objects;

	return QString::fromStdString(std::string(json.dump()));
}

void SpaceWidget::exportToFile(QString file)
{
	QFile targetFile(file);
	if(targetFile.open(QIODevice::WriteOnly))
	{
		targetFile.write(factory->generateStlFile(&vertexData));
	}
}

void SpaceWidget::reset()
{
	Plane *basePlane = planes.at(0);
	planes.clear();
	setTopPlane(basePlane);

	factory->deleteAllItems();
	update();
}

void SpaceWidget::reallocateItems()
{
	vertexData.clear();
	//generates buffer data
	vertexData = factory->generateBuffer();

	vertexBuffer.bind();
	vertexBuffer.allocate(vertexData.data(), vertexData.size() * sizeof(Vertex));
	vertexBuffer.release();

	update();
}

void SpaceWidget::reallocatePlanes()
{
	planeVertexData.clear();

	for(int i = 0; i < planes.size(); i++ )
	{
		allocateNewPlane();
	}
}

void SpaceWidget::allocateNewPlane(double planeSize)
{
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

Plane *SpaceWidget::getBasePlane()
{
	return planes.at(0);
}

void SpaceWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	QOpenGLWidget::mousePressEvent(event);
}

void SpaceWidget::mouseReleaseEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	QOpenGLWidget::mouseReleaseEvent(event);
}

void SpaceWidget::mouseMoveEvent(QMouseEvent *event)
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

void SpaceWidget::wheelEvent(QWheelEvent *event)
{
	QOpenGLWidget::wheelEvent(event);

	QVector3D translation = camera.forward() * event->delta();
	static const float transSpeed = 0.05f;
	camera.translate(transSpeed * translation);

	// redraw
	QOpenGLWidget::update();
}

void SpaceWidget::keyPressEvent(QKeyEvent *event)
{
	QOpenGLWidget::keyPressEvent(event);
}

void SpaceWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	//item buffer

	vertexProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/src/shaders/item.vert");
	vertexProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/src/shaders/item.frag");
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
	lightByNormals = vertexProgram.uniformLocation("lightByNormals");

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
	vertexProgram.setAttributeBuffer(1, GL_FLOAT, Vertex::normalOffset(), Vertex::NormalTupleSize, Vertex::stride());
	vertexBufferObject.release();
	vertexBuffer.release();


	//planes buffer

	planesProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/src/shaders/planes.vert");
	planesProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/src/shaders/planes.frag");
	planesProgram.link();
	planesProgram.bind();

	planesParameter.itemToSpace = planesProgram.uniformLocation("itemToSpace");
	planesParameter.worldToCamera = planesProgram.uniformLocation("worldToCamera");
	planesParameter.cameraToView = planesProgram.uniformLocation("cameraToView");
	planesParameter.itemToRotate = planesProgram.uniformLocation("itemToRotate");
	planeColor = planesProgram.uniformLocation("planeColor");

	planesProgram.release();


	planeBuffer.create();
	planeBuffer.bind();
	planeBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	planeBuffer.allocate(planeVertexData.data(), planeVertexData.size() * sizeof(Vertex));

	planeBufferObject.create();
	planeBufferObject.bind();

	planesProgram.enableAttributeArray(0);
	planesProgram.setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());

	planeBuffer.release();
	planeBufferObject.release();
}

void SpaceWidget::resizeGL(int width, int height)
{
	static float near = 0.1;
	static float far = 1000;

	projection.setToIdentity();
	projection.perspective(45.0f, width / float(height), near, far);
}

void SpaceWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vertexProgram.bind();
	vertexProgram.setUniformValue(vertexParameter.worldToCamera, camera.toMatrix());
	vertexProgram.setUniformValue(vertexParameter.cameraToView, projection);
	vertexProgram.setUniformValue(selectedItemColor, Settings::selectedFaceColor);
	vertexProgram.setUniformValue(lightByNormals, Settings::colorByNormals);

	QMatrix4x4 mtr;
	mtr.rotate(itemRotation.normalized());
	vertexProgram.setUniformValue(vertexParameter.itemToRotate, mtr);

	//items
	vertexBufferObject.bind();
	vertexProgram.setUniformValue(selectedTransparentValue, 1.0f);

	foreach (Item *item, objectsInSpace)
	{
		int currentIndex = item->getItemIndex();

		// positions are already global in the buffer, but if I remove the matrix from shader, planes stop working
		//vertexProgram.setUniformValue(vertexParameter.itemToSpace, item->toMatrix());
		QMatrix4x4 mat;
		mat.setColumn(0, QVector4D(1,0,0,0));
		mat.setColumn(1, QVector4D(0,1,0,0));
		mat.setColumn(2, QVector4D(0,0,1,0));
		mat.setColumn(3, QVector4D(0,0,0,1));
		vertexProgram.setUniformValue(vertexParameter.itemToSpace, mat);

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

	planesProgram.setUniformValue(vertexParameter.itemToRotate, mtr);

	for(int i = 0; i < planes.size(); i++)
	{
		if(planes.at(i)->checkState(0) == Qt::Checked)
		{
			planesProgram.setUniformValue(planesParameter.itemToSpace, planes.at(i)->toMatrix());
			glDrawArrays(GL_TRIANGLES, i * 6, 6);
		}
	}

	planeBufferObject.release();

	planesProgram.release();
}
