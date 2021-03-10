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

QByteArray SpaceFactory::generateStlFile(std::vector<Vertex> *vertexData)
{
	QByteArray file;

	file.fill('k',80); // 80 byte header


	uint32_t length = vertexData->size() / 3;
	char *size = (char*)&length;

	//4 byte length space
	for(int a = 0; a < 4; a++)
		file.append(size[a]);

	//triangles
	for(uint i = 0; i < vertexData->size(); i+=3)
	{
		//normal vector - 12 bytes
		for(int a = 0; a < 12; a++)
			file += uint8_t(0);

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
	calculateBooleanIgl(&vertexesInItems);

	//copy final data to the buffer
	for(uint32_t i = 0; i < vertexesInItems.size(); i++)
	{
		//add item datasize since it's only now final
		if(Item *item = dynamic_cast<Item*>(itemsInSpace->at(i)))
		{
			item->setDataSize(vertexesInItems.at(i).size());
		}
		vertexBuffer->insert(vertexBuffer->end(), vertexesInItems.at(i).begin(), vertexesInItems.at(i).end());
	}
}

void SpaceFactory::calculateBooleanIgl(std::vector<std::vector<Vertex>> *triangularizedVertexData)
{
	std::vector<Eigen::Matrix3Xd> vertexesLocations;
	std::vector<Eigen::Matrix3Xi> vertexesIndexes;
/*
	for(int i = 0; i < itemsInSpace->size(); i++)
	{
		Item *item = itemsInSpace->at(i);
		if(!item->isExtruded())
		{
			//ensure that indexes in objectsInSpace and vertexLocations remain the equivalent
			vertexesLocations.push_back(Eigen::Matrix3Xd(0));
			vertexesIndexes.push_back(Eigen::Matrix3Xi(0));
		}
		else
		{
			std::vector<Vertex> vertexes = triangularizedVertexData->at(i);

			uint32_t vertexCount = vertexes.size();
			vertexesLocations.push_back(Eigen::Matrix3Xd(vertexCount));

			uint32_t triangleCount = vertexes.size() / 3;
			vertexesIndexes.push_back(Eigen::Matrix3Xi(triangleCount));

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
		if(itemsInSpace->at(i)->isExtruded() & itemsInSpace->at(i)->isAdditive())
		{
			for(uint32_t ii = 0; ii < vertexesLocations.size(); ii++)
			{
				//every extruded subtractive item
				if(itemsInSpace->at(i)->isExtruded() & !itemsInSpace->at(i)->isAdditive())
				{
					igl::copyleft::cgal::mesh_boolean(
								vertexesLocations.at(i), vertexesIndexes.at(i),
								vertexesLocations.at(ii), vertexesIndexes.at(ii),
								igl::MESH_BOOLEAN_TYPE_MINUS,
								vertexesLocations.at(i), vertexesIndexes.at(i)
													  );
				}
			}
		}
	}*/

	//return the resulting vertexes and add non extruded items
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

	connect(newItem, &Item::getBasePlane,
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
		Item *item = new Item(plane, polygons, sketch);

		item->setText(0, "item " + QString::number(itemsInSpace->size()));

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

		emit reallocateItems();
	}
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
		int baseSize = (vertexes->size() / 2) + 1;

		base[0].assign(vertexes->data(), vertexes->data()+baseSize-1);
		base[1].assign(vertexes->data() + baseSize-1, vertexes->data()+vertexes->size());

		//calculate bases
		for (int a = 0; a < 2 ; a++)
		{
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
