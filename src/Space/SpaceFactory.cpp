#include "SpaceFactory.h"

SpaceFactory *SpaceFactory::instance = nullptr;

void SpaceFactory::init(QVector<Item*> *objectsInSpace, QVector<Plane*> *planes, QObject *glWidget)
{
	SpaceFactory::instance = new SpaceFactory(objectsInSpace, planes, glWidget);
}

SpaceFactory *SpaceFactory::getInstance()
{
	return SpaceFactory::instance;
}

QVector3D SpaceFactory::getNormalVector(QVector3D objectCenter, const Vertex *vertex1, const Vertex *vertex2, const Vertex *vertex3)
{
	QVector3D sideVector = vertex2->position() - vertex1->position();
	sideVector.normalize();

	QVector3D triangleCenter = QVector3D(
			(vertex1->position().x() + vertex2->position().x() + vertex3->position().x()) / 3,
			(vertex1->position().y() + vertex2->position().y() + vertex3->position().y()) / 3,
			(vertex1->position().z() + vertex2->position().z() + vertex3->position().z()) / 3
				);

	QVector3D upwardVector = triangleCenter - objectCenter;
	upwardVector.normalize();

	std::vector<QVector3D> possibleVectors;

	possibleVectors.push_back(QVector3D(sideVector.x(), -sideVector.z(), sideVector.y()));
	possibleVectors.push_back(QVector3D(sideVector.x(), sideVector.z(), -sideVector.y()));

	possibleVectors.push_back(QVector3D(-sideVector.z(), sideVector.y(), sideVector.x()));
	possibleVectors.push_back(QVector3D(sideVector.z(), sideVector.y(), -sideVector.x()));

	possibleVectors.push_back(QVector3D(-sideVector.y(), sideVector.x(), sideVector.z()));
	possibleVectors.push_back(QVector3D(sideVector.y(), -sideVector.x(), sideVector.z()));

	QVector3D targetPoint = triangleCenter + upwardVector;

	std::vector<double> distances;
	for(uint32_t i = 0; i < possibleVectors.size(); i++)
	{
		distances.push_back(targetPoint.distanceToPoint(triangleCenter + possibleVectors.at(i).normalized()));
	}

	std::min_element(distances.begin(), distances.end());
	return possibleVectors.at(std::distance(std::begin(distances), std::min_element(std::begin(distances), std::end(distances))));
}

QByteArray SpaceFactory::generateStlFile(std::vector<Vertex> *vertexData)
{
	QVector3D objectCenter;
	uint32_t vertexCount = vertexData->size();
	for(uint32_t i = 0; i < vertexCount; i++)
	{
		objectCenter += vertexData->at(i).position();
	}
	objectCenter.setX(objectCenter.x() / vertexCount);
	objectCenter.setX(objectCenter.y() / vertexCount);
	objectCenter.setX(objectCenter.z() / vertexCount);


	QByteArray file;

	file.fill('k',80); // 80 byte header


	uint32_t length = vertexCount / 3;
	char *size = (char*)&length;

	//4 byte length space
	for(int a = 0; a < 4; a++)
		file.append(size[a]);

	//triangles
	for(uint i = 0; i < vertexCount; i+=3)
	{
		//normal vector - 12 bytes
		file += vectorToByteArray(getNormalVector(objectCenter, &vertexData->at(i), &vertexData->at(i+1), &vertexData->at(i+2)));

		file += vertexData->at(i).toByteArray();	//vertex 0 - 12 bytes
		file += vertexData->at(i +1).toByteArray(); //vertex 1 - 12 bytes
		file += vertexData->at(i +2).toByteArray(); //vertex 2 - 12 bytes

		//Attribute byte count - 0 (legacy specification) - 2 bytes
		file += u_int8_t(0);
		file += u_int8_t(0);
	}

	return file;
}

void SpaceFactory::generateBuffer(std::vector<Vertex> *vertexBuffer)
{
	//vector of (all vertexes in an item)
	std::vector<std::vector<Vertex>> vertexesInItems;
	for(int i = 0; i < itemsInSpace->size(); i++)
		vertexesInItems.push_back(std::vector<Vertex>());

	//triangularize exstruded items
	for(uint32_t i = 0; i < vertexesInItems.size(); i++)
	{
		if(itemsInSpace->at(i)->isExtruded())
		{
			vertexesInItems.at(i) = triangularizeItem(itemsInSpace->at(i));
		}
		else
		{
			//triangularize nonExtruded items and setDataSize since it's finalized
			QVector<Polygon*> *polygons = itemsInSpace->at(i)->getPolygons();
			for(int ii = 0; ii < polygons->size(); ii++)
			{
				std::vector<Vertex> triangularized = triangularizePolygon(polygons->at(ii)->getQpolygon());

				polygons->at(ii)->setDataSize(triangularized.size());
				vertexesInItems.at(i) = triangularized;
			}
		}
	}

	//calculates bollean operations
	calculateBoolean(&vertexesInItems);

	uint32_t itemIndex = 0;
	//copy final data to the buffer
	for(uint32_t i = 0; i < vertexesInItems.size(); i++)
	{
		if(itemsInSpace->at(i)->isExtruded()) itemsInSpace->at(i)->setDataSize(vertexesInItems.at(i).size());
		itemsInSpace->at(i)->setItemIndex(itemIndex);

		itemIndex += vertexesInItems.at(i).size();
		vertexBuffer->insert(vertexBuffer->end(), vertexesInItems.at(i).begin(), vertexesInItems.at(i).end());
	}
}

void SpaceFactory::calculateBoolean(std::vector<std::vector<Vertex>> *triangularizedVertexData)
{
	std::vector<Eigen::MatrixXd> vertexesLocations;
	std::vector<Eigen::MatrixXi> vertexesIndexes;

	for(int i = 0; i < itemsInSpace->size(); i++)
	{
		Item *item = itemsInSpace->at(i);
		if(!item->isExtruded())
		{
			//ensure that indexes in objectsInSpace and vertexLocations remain the equivalent
			vertexesLocations.push_back(Eigen::MatrixXd(1,3));
			vertexesIndexes.push_back(Eigen::MatrixXi(1,3));
		}
		else
		{
			std::vector<Vertex> vertexes = triangularizedVertexData->at(i);

			uint32_t vertexCount = vertexes.size();
			vertexesLocations.push_back(Eigen::MatrixXd(vertexCount, 3));

			uint32_t triangleCount = vertexes.size() / 3;
			vertexesIndexes.push_back(Eigen::MatrixXi(triangleCount, 3));

			//create a location mesh for this item of vertexes size
			for(uint32_t ii = 0; ii < vertexes.size(); ii++)
			{
				QVector3D pos = vertexes.at(ii).position();
				vertexesLocations.at(i)(ii, 0) = pos.x();
				vertexesLocations.at(i)(ii, 1) = pos.y();
				vertexesLocations.at(i)(ii, 2) = pos.z();
			}

			//create index mesh of vertex size
			for(uint32_t ii = 0; ii < triangleCount; ii++)
			{
				vertexesIndexes.at(i)(ii, 0) = ii*3;
				vertexesIndexes.at(i)(ii, 1) = ii*3 + 1;
				vertexesIndexes.at(i)(ii, 2) = ii*3 + 2;
			}
		}
	}

	//for every extruded additive item, subtract every extruded subtractive item
	for(uint32_t i = 0; i < vertexesLocations.size(); i++)
	{
		//every extruded additive item
		if(itemsInSpace->at(i)->isExtruded())
		{
			triangularizedVertexData->at(i).clear();

			if(itemsInSpace->at(i)->isAdditive())
			{
				for(uint32_t ii = 0; ii < vertexesLocations.size(); ii++)
				{
					//every extruded subtractive item
					if(!itemsInSpace->at(i)->isAdditive())
					{
						qDebug() << "subtractive";
						igl::copyleft::cgal::mesh_boolean(
									vertexesLocations.at(i), vertexesIndexes.at(i),
									vertexesLocations.at(ii), vertexesIndexes.at(ii),
									igl::MESH_BOOLEAN_TYPE_MINUS,
									vertexesLocations.at(i), vertexesIndexes.at(i)
														  );
					}
				}

				// i = object, ii = index value, iii = array index (x, y, z)
				for(uint32_t ii = 0; ii < vertexesIndexes.at(i).rows(); ii++)
				{
					for(int iii = 0; iii < 3; iii++)
					{
						int locationIndex = vertexesIndexes.at(i)(ii,iii);
						double x, y, z;
						x = vertexesLocations.at(i)(locationIndex, 0);
						y = vertexesLocations.at(i)(locationIndex, 1);
						z = vertexesLocations.at(i)(locationIndex, 2);

						triangularizedVertexData->at(i).push_back(Vertex(x,y,z));
					}
				}
			}
		}
	}
}

QByteArray SpaceFactory::vectorToByteArray(QVector3D vector)
{
	QByteArray result;

	float point[3] = {
		vector.x(),
		vector.y(),
		vector.z()
	};

	for(int a = 0; a < 3; a++)
	{
		char *pointer = (char*)&point[a];
		for(int i = 0; i < 4; i++)
			result += pointer[i];
	}

	return result;
}

void SpaceFactory::recieveTargetItem(QTreeWidgetItem *item)
{
	targetItem = item;
}

Item *SpaceFactory::loadItem(std::string file)
{
	nlohmann::json input = nlohmann::json::parse(file);

	QString sketch = QString::fromStdString(input["sketch"]);

	Item *newItem = new Item();

	newItem->setPolygons(emit generatePolygons(sketch));

	int polygon = input["extrusionPolygon"];
	Extrusion extrusion;
	bool extruded = false;
	if(polygon != -1)
	{
		extruded = true;
		extrusion.length = input["extrusion"]["length"];
		extrusion.additive = input["extrusion"]["additive"];
		extrusion.direction = input["extrusion"]["direction"];
	}

	newItem->loadData(QString::fromStdString(input["basePlane"]), extrusion, extruded, polygon);

	connect(newItem, &Item::getOriginalPlane,
			this, &SpaceFactory::getBasePlane
			);
	connect(newItem, &Item::planeAdded,
			this, &SpaceFactory::addPlane
			);
	connect(newItem, &Item::deletePlane,
			this, &SpaceFactory::deletePlane
			);

	return newItem;
}

void SpaceFactory::addNewItem(std::vector<QPolygonF> polygons, QString sketch)
{
	if(Plane *plane = dynamic_cast<Plane*>(targetItem))
	{
		qDebug() << "new item from plane";
		Item *item = new Item(plane, polygons, sketch);

		item->setText(0, "Drawing " + QString::number(itemsInSpace->size()));

		connect(item, &Item::planeAdded,
				this, &SpaceFactory::addPlane
				);
		connect(item, &Item::deletePlane,
				this, &SpaceFactory::deletePlane
				);

		addItem(item);
	}
	else if(Item *item = dynamic_cast<Item*>(targetItem))
	{
		item->setPolygons(polygons);
		item->setSketch(sketch);
	}
	emit reallocateItems();
}

void SpaceFactory::addItem(Item *item, bool reallocate)
{
	itemsInSpace->append(item);

	connect(item, &Item::updateData,
			this, &SpaceFactory::reallocateItems
			);
	if(reallocate) emit reallocateItems();
}

void SpaceFactory::deleteItem(Item *item, bool reallocate)
{
	itemsInSpace->remove(itemsInSpace->indexOf(item));
	delete item;
	if(reallocate) emit reallocateItems();
}

void SpaceFactory::deleteAllItems()
{
	for (int i = itemsInSpace->size()-1; i >= 0; i--)
	{
		deleteItem(itemsInSpace->at(i), false);
	}
	emit reallocateItems();
}

void SpaceFactory::addPlane(Plane *plane)
{
	planes->append(plane);
	plane->setItemIndex(itemsInSpace->indexOf(dynamic_cast<Item*>(plane->parent())));

	emit allocateNewPlane();
}

void SpaceFactory::deletePlane(Plane *plane)
{
	if(plane == nullptr) return;

	for (int i = 0; i < plane->childCount(); i++)
	{
		if(Item *item = dynamic_cast<Item*>(plane->child(i)))
			deleteItem(item);
	}

	planes->removeAll(plane);
	delete plane;
}

SpaceFactory::SpaceFactory(QVector<Item*> *objectsInSpace, QVector<Plane*> *planes, QObject *glWidget)
{
	this->itemsInSpace = objectsInSpace;
	this->planes = planes;
	this->glWidget = glWidget;
}

std::vector<Vertex> SpaceFactory::triangularizePolygon(QPolygonF polygon)
{
	std::vector<double> coordinates;
	std::vector<QPointF> points = polygon.toStdVector();
	std::vector<Vertex> vertexData;

	for(uint32_t i = 0; i < points.size(); i++)
	{
		coordinates.push_back(points.at(i).x());
		coordinates.push_back(points.at(i).y());
	}

	delaunator::Delaunator d(coordinates);
	for(std::size_t i = 0; i < d.triangles.size(); i+=3)
	{
		vertexData.push_back(Vertex(
			d.coords[2 * d.triangles[i]],
			d.coords[2 * d.triangles[i] + 1],
			0
		));

		vertexData.push_back(Vertex(
			d.coords[2 * d.triangles[i + 1]],
			d.coords[2 * d.triangles[i + 1] + 1],
			0
		));

		vertexData.push_back(Vertex(
			d.coords[2 * d.triangles[i + 2]],
			d.coords[2 * d.triangles[i + 2] + 1],
			0
		));
	}

	return vertexData;
}

std::vector<Vertex> SpaceFactory::triangularizeItem(Item *item)
{
	std::vector<Vertex> vertexData;

	if(item->isExtruded())
	{
		std::vector<Vertex> *vertexes = item->getExtrudedVertexes();
		std::vector<Vertex> base[2];
		int baseSize = (vertexes->size() / 2);

		for(int i = 0; i < baseSize; i++)
			base[0].push_back(vertexes->at(i));
		for(int i = 0; i < baseSize; i++)
			base[1].push_back(vertexes->at(baseSize + i));


		//calculate bases
		for (int a = 0; a < 2 ; a++)
		{
			int baseHeight = base[a].at(0).position().z();

			std::vector<double> coordinates;
			for(uint32_t i = 0; i < base[a].size(); i++)
			{
				coordinates.push_back(base[a].at(i).position().x());
				coordinates.push_back(base[a].at(i).position().y());
			}

			//calculate vertexes for both bases
			delaunator::Delaunator d(coordinates);
			for(std::size_t i = 0; i < d.triangles.size(); i+=3)
			{
				vertexData.push_back(Vertex(
					d.coords[2 * d.triangles[i]],
					d.coords[2 * d.triangles[i] + 1],
					baseHeight
				));

				vertexData.push_back(Vertex(
					d.coords[2 * d.triangles[i + 1]],
					d.coords[2 * d.triangles[i + 1] + 1],
					baseHeight
				));

				vertexData.push_back(Vertex(
					d.coords[2 * d.triangles[i + 2]],
					d.coords[2 * d.triangles[i + 2] + 1],
					baseHeight
				));
			}
		}

		//calculate faces
		for (uint32_t i = 0; i < base[0].size() - 1; i++)
		{
			vertexData.push_back(base[0].at(i));
			vertexData.push_back(base[0].at(i + 1));
			vertexData.push_back(base[1].at(i));

			vertexData.push_back(base[0].at(i + 1));
			vertexData.push_back(base[1].at(i + 1));
			vertexData.push_back(base[1].at(i));
		}
		//calculate closing faces
		{
			int lastIndex = base[0].size() - 1;

			vertexData.push_back(base[0].at(lastIndex));
			vertexData.push_back(base[0].at(0));
			vertexData.push_back(base[1].at(lastIndex));

			vertexData.push_back(base[0].at(0));
			vertexData.push_back(base[1].at(0));
			vertexData.push_back(base[1].at(lastIndex));
		}
	}

	return vertexData;
}
