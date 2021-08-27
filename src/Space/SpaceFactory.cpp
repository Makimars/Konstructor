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
	uint32_t vertexCount = vertexData->size();
	std::vector<Vertex> vertexes;

	//convert to real size
	for(uint32_t i = 0; i < vertexCount; i++)
	{
		vertexes.push_back(vertexData->at(i));
		vertexes.at(i).setPosition(vertexes.at(i).position() * Settings::planeToSpaceRatio);
	}


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
		file += vectorToByteArray(vertexes.at(i).normal());

		file += vertexes.at(i).toByteArray();	//vertex 0 - 12 bytes
		file += vertexes.at(i +1).toByteArray(); //vertex 1 - 12 bytes
		file += vertexes.at(i +2).toByteArray(); //vertex 2 - 12 bytes

		//Attribute byte count - 0 (legacy specification) - 2 bytes
		file += u_int8_t(0);
		file += u_int8_t(0);
	}

	return file;
}

void SpaceFactory::generateOffFile(std::vector<Vertex> *vertexData, QString filePath)
{
#ifndef NO_CGAL
	std::vector<Vertex> vertexes;

	//convert to real size
	for(uint32_t i = 0; i < vertexData->size(); i++)
	{
		vertexes.push_back(vertexData->at(i));
		vertexes.at(i).setPosition(vertexes.at(i).position() * Settings::planeToSpaceRatio);
	}


	namespace PMP = CGAL::Polygon_mesh_processing;
	namespace params = PMP::parameters;

	typedef CGAL::Exact_predicates_inexact_constructions_kernel          K;
	typedef CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3>      Polyhedron;

	std::vector<K::Point_3> points;
	std::vector<std::vector<std::size_t>> polygons;
	std::vector<Polyhedron> meshes;
	std::vector<std::vector<Vertex>> result;

	QVector<QVector3D> locations;

	//copy all unique vertexes
	for(uint32_t ii = 0; ii < vertexes.size(); ii++)
	{
		QVector3D vec = vertexes.at(ii).position();
		if (locations.indexOf(vec) == -1)
		{
			locations.push_back(vec);
			points.push_back(K::Point_3(vec.x(), vec.y(), vec.z()));
		}
	}

	uint32_t triangleCount = vertexes.size() / 3;

	//create index mesh of vertex size
	for(uint32_t ii = 0; ii < triangleCount; ii++)
	{
		polygons.push_back(std::vector<std::size_t>());
		//triangle
		polygons.at(ii).push_back(locations.indexOf(vertexes.at(ii*3).position()));
		polygons.at(ii).push_back(locations.indexOf(vertexes.at(ii*3 +1).position()));
		polygons.at(ii).push_back(locations.indexOf(vertexes.at(ii*3 +2).position()));
	}

	CGAL::Polygon_mesh_processing::orient_polygon_soup(points, polygons);
	Polyhedron mesh;
	CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, polygons, mesh);
	// Number the faces because 'orient_to_bound_a_volume' needs a face <--> index map
	int index = 0;
	for(Polyhedron::Face_iterator fb=mesh.facets_begin(), fe=mesh.facets_end(); fb!=fe; ++fb)
	  fb->id() = index++;
	if(CGAL::is_closed(mesh))
	{
		CGAL::Polygon_mesh_processing::orient_to_bound_a_volume(mesh);
	}

	//convert via file
	std::ofstream out(filePath.toStdString());
	out.precision(17);
	out << mesh;
	out.close();
#endif
}

std::vector<Vertex> SpaceFactory::generateItemBuffer()
{
	return SpaceProcessor::getInstance()->generateItemBuffer(itemsInSpace);
}

std::vector<Vertex> SpaceFactory::generateLinesBuffer()
{
	return SpaceProcessor::getInstance()->generateLinesBuffer(itemsInSpace);
}

std::vector<Vertex> SpaceFactory::generatePlanesBuffer()
{
	return SpaceProcessor::getInstance()->generatePlanesBuffer(planes);
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
