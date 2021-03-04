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

void SpaceFactory::generateItemVertexDataToBuffer(Item *item, std::vector<Vertex> *vertexData)
{
	if(item->isExtruded())
	{
		std::vector<Vertex> *vertexes = item->getExtrudedVertexes();
		std::vector<Vertex> base[2];
		int baseSize = (vertexes->size() / 2) + 1;

		base[0].assign(vertexes->data(), vertexes->data()+baseSize-1);
		base[1].assign(vertexes->data() + baseSize-1, vertexes->data()+vertexes->size());
		int dataSize = 0;

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
				vertexData->push_back(Vertex(
					d.coords[2 * d.triangles[i]],
					d.coords[2 * d.triangles[i] + 1],
					0
				));

				vertexData->push_back(Vertex(
					d.coords[2 * d.triangles[i + 1]],
					d.coords[2 * d.triangles[i + 1] + 1],
					0
				));

				vertexData->push_back(Vertex(
					d.coords[2 * d.triangles[i + 2]],
					d.coords[2 * d.triangles[i + 2] + 1],
					0
				));

				dataSize += 3;
			}
		}

		//calculate faces
		for (uint32_t i = 0; i < base[0].size() - 1; i++)
		{
			vertexData->push_back(base[0].at(i));
			vertexData->push_back(base[0].at(i + 1));
			vertexData->push_back(base[1].at(i));

			vertexData->push_back(base[0].at(i + 1));
			vertexData->push_back(base[1].at(i + 1));
			vertexData->push_back(base[1].at(i));

			dataSize += 6;
		}
		//calculate closing faces
		{
			int lastIndex = base[0].size() - 1;

			vertexData->push_back(base[0].at(lastIndex));
			vertexData->push_back(base[0].at(0));
			vertexData->push_back(base[1].at(lastIndex));

			vertexData->push_back(base[0].at(0));
			vertexData->push_back(base[1].at(0));
			vertexData->push_back(base[1].at(lastIndex));

			dataSize += 6;
		}

		item->setDataSize(dataSize);
	}
	else
	{
		QVector<Polygon*> *polygons = item->getPolygons();
		for(int a = 0; a < polygons->size(); a++)
		{
			std::vector<double> coordinates;
			std::vector<QPointF> points = polygons->at(a)->getPoints();

			for(uint32_t i = 0; i < points.size(); i++)
			{
				coordinates.push_back(points.at(i).x());
				coordinates.push_back(points.at(i).y());
			}

			delaunator::Delaunator d(coordinates);
			for(std::size_t i = 0; i < d.triangles.size(); i+=3)
			{
				vertexData->push_back(Vertex(
					d.coords[2 * d.triangles[i]],
					d.coords[2 * d.triangles[i] + 1],
					0
				));

				vertexData->push_back(Vertex(
					d.coords[2 * d.triangles[i + 1]],
					d.coords[2 * d.triangles[i + 1] + 1],
					0
				));

				vertexData->push_back(Vertex(
					d.coords[2 * d.triangles[i + 2]],
					d.coords[2 * d.triangles[i + 2] + 1],
					0
				));

			}
			polygons->at(a)->setDataSize(d.triangles.size());
		}
	}
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

		item->setText(0, "item " + QString::number(objectsInSpace->size()));

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

void SpaceFactory::addItem(Item *item)
{
	objectsInSpace->append(item);

	connect(item, &Item::updateData,
			this, &SpaceFactory::reallocateItems
			);

	emit allocateNewItem(item);
}

void SpaceFactory::deleteItem(Item *item)
{
	objectsInSpace->remove(objectsInSpace->indexOf(item));
	delete item;
	emit reallocateItems();
}

void SpaceFactory::deleteAllItems()
{
	for (int i = objectsInSpace->size()-1; i >= 0; i--)
	{
		deleteItem(objectsInSpace->at(i));
	}
	emit reallocateItems();
}

void SpaceFactory::addPlane(Plane *plane)
{
	planes->append(plane);
	plane->setItemIndex(objectsInSpace->indexOf(dynamic_cast<Item*>(plane->parent())));

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
	this->objectsInSpace = objectsInSpace;
	this->planes = planes;
	this->glWidget = glWidget;
}
