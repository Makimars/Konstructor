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
		file += vectorToByteArray(vertexData->at(i).normal());

		file += vertexData->at(i).toByteArray();	//vertex 0 - 12 bytes
		file += vertexData->at(i +1).toByteArray(); //vertex 1 - 12 bytes
		file += vertexData->at(i +2).toByteArray(); //vertex 2 - 12 bytes

		//Attribute byte count - 0 (legacy specification) - 2 bytes
		file += u_int8_t(0);
		file += u_int8_t(0);
	}

	return file;
}

std::vector<Vertex> SpaceFactory::generateBuffer()
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

#if defined(CGAL_BOOLEAN) || defined(CGAL_BOOLEAN)
	vertexesInItems = calculateBoolean(&vertexesInItems);
#endif

	uint32_t itemIndex = 0;
	std::vector<Vertex> finalBufferVertexes;
	//copy final data to the buffer
	for(uint32_t i = 0; i < vertexesInItems.size(); i++)
	{
		if(itemsInSpace->at(i)->isExtruded()) itemsInSpace->at(i)->setDataSize(vertexesInItems.at(i).size());
		itemsInSpace->at(i)->setItemIndex(itemIndex);

		itemIndex += vertexesInItems .at(i).size();
		finalBufferVertexes.insert(finalBufferVertexes.end(), vertexesInItems.at(i).begin(), vertexesInItems.at(i).end());
	}

	return finalBufferVertexes;
}

void SpaceFactory::assignNormals(std::vector<Vertex> *vertexData)
{
	QVector3D objectCenter;
	for(uint32_t i = 0; i < vertexData->size(); i++)
	{
		objectCenter += vertexData->at(i).position();
	}
	objectCenter.setX(objectCenter.x() / vertexData->size());
	objectCenter.setY(objectCenter.y() / vertexData->size());
	objectCenter.setZ(objectCenter.z() / vertexData->size());

	for (uint32_t i = 0; i < vertexData->size(); i+=3)
	{
		QVector3D normal = Vertex::generateNormalVector(objectCenter, &vertexData->at(i), &vertexData->at(i+1), &vertexData->at(i+2));
		normal.normalize();

		vertexData->at(i).setNormal(normal);
		vertexData->at(i+1).setNormal(normal);
		vertexData->at(i+2).setNormal(normal);
	}
}

#ifdef IGL_BOOLEAN
std::vector<std::vector<Vertex>> SpaceFactory::calculateBoolean(const std::vector<std::vector<Vertex>> *triangularizedVertexData) const
{
	std::vector<Eigen::MatrixXd> vertexesLocations;
	std::vector<Eigen::MatrixXi> vertexesIndexes;

	std::vector<std::vector<Vertex>> result;

	//convert items to Eigen matrix format
	for(int i = 0; i < itemsInSpace->size(); i++)
	{
		result.push_back(std::vector<Vertex>());
		Item *item = itemsInSpace->at(i);

		if(!item->isExtruded())
		{
			result.at(i) = triangularizedVertexData->at(i);
			//ensure that indexes in objectsInSpace and vertexLocations remain the equivalent
			vertexesLocations.push_back(Eigen::MatrixXd(1,3));
			vertexesIndexes.push_back(Eigen::MatrixXi(1,3));
		}
		else
		{
			std::vector<Vertex> vertexes = triangularizedVertexData->at(i);
			QVector<QVector3D> locations;

			//copy all unique vertexes
			for(uint32_t ii = 0; ii < vertexes.size(); ii++)
			{
				QVector3D vec = vertexes.at(ii).position();
				if (locations.indexOf(vec) == -1)
				{
					locations.append(vec);
				}
			}

			vertexesLocations.push_back(Eigen::MatrixXd(locations.size(),3));
			for(int ii = 0; ii < locations.size(); ii++)
			{
				QVector3D vec = locations.at(ii);
				vertexesLocations.at(i)(ii, 0) = vec.x();
				vertexesLocations.at(i)(ii, 1) = vec.y();
				vertexesLocations.at(i)(ii, 2) = vec.z();
			}

			uint32_t triangleCount = vertexes.size() / 3;
			vertexesIndexes.push_back(Eigen::MatrixXi(triangleCount, 3));

			//create index mesh of vertex size
			for(uint32_t ii = 0; ii < triangleCount; ii++)
			{
				vertexesIndexes.at(i)(ii, 0) = locations.indexOf(vertexes.at(ii*3).position());
				vertexesIndexes.at(i)(ii, 1) = locations.indexOf(vertexes.at(ii*3 +1).position());
				vertexesIndexes.at(i)(ii, 2) = locations.indexOf(vertexes.at(ii*3 +2).position());
			}
		}
	}



	//for every extruded additive item, subtract every extruded subtractive item
	for(int i = 0; i < itemsInSpace->size(); i++)
	{

		Item *currentItem = itemsInSpace->at(i);

		//every extruded additive item
		if(currentItem->isExtruded())
		{
			if(currentItem->isAdditive())
			{

				for(uint32_t ii = 0; ii < vertexesLocations.size(); ii++)
				{
					Item *insideItem = itemsInSpace->at(ii);

					//every extruded subtractive item
					if((!insideItem->isAdditive()) & insideItem->isExtruded())
					{
						igl::copyleft::cgal::mesh_boolean(
									vertexesLocations.at(i), vertexesIndexes.at(i),
									vertexesLocations.at(ii), vertexesIndexes.at(ii),
									igl::MeshBooleanType::MESH_BOOLEAN_TYPE_MINUS,
									vertexesLocations.at(i), vertexesIndexes.at(i)
														  );

						qDebug() << ii;
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

						result.at(i).push_back(Vertex(x,y,z));
					}
				}
			}
		}
	}

	return result;
}
#endif

#ifdef CGAL_BOOLEAN
std::vector<std::vector<Vertex>> SpaceFactory::calculateBoolean(const std::vector<std::vector<Vertex>> *triangularizedVertexData) const
{
	namespace PMP = CGAL::Polygon_mesh_processing;
	namespace params = PMP::parameters;

	typedef CGAL::Exact_predicates_inexact_constructions_kernel          K;
	typedef CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3>      Polyhedron;

	std::vector<std::vector<K::Point_3>> points;
	std::vector<std::vector<std::vector<std::size_t>>> polygons;
	std::vector<Polyhedron> meshes;
	std::vector<std::vector<Vertex>> result;

	//convert to CGAL
	for(int itemIndex = 0; itemIndex < itemsInSpace->size(); itemIndex++)
	{
		Item *item = itemsInSpace->at(itemIndex);

		result.push_back(std::vector<Vertex>());
		points.push_back(std::vector<K::Point_3>());
		polygons.push_back(std::vector<std::vector<std::size_t>>());
		meshes.push_back(Polyhedron());

		if(!item->isExtruded())
		{
			result.at(itemIndex) = triangularizedVertexData->at(itemIndex);
		}
		else
		{
			std::vector<Vertex> vertexes = triangularizedVertexData->at(itemIndex);
			QVector<QVector3D> locations;

			//copy all unique vertexes
			for(uint32_t ii = 0; ii < vertexes.size(); ii++)
			{
				QVector3D vec = vertexes.at(ii).position();
				if (locations.indexOf(vec) == -1)
				{
					locations.push_back(vec);
					points.at(itemIndex).push_back(K::Point_3(vec.x(), vec.y(), vec.z()));
				}
			}

			uint32_t triangleCount = vertexes.size() / 3;

			//create index mesh of vertex size
			for(uint32_t ii = 0; ii < triangleCount; ii++)
			{
				polygons.at(itemIndex).push_back(std::vector<std::size_t>());
				//triangle
				polygons.at(itemIndex).at(ii).push_back(locations.indexOf(vertexes.at(ii*3).position()));
				polygons.at(itemIndex).at(ii).push_back(locations.indexOf(vertexes.at(ii*3 +1).position()));
				polygons.at(itemIndex).at(ii).push_back(locations.indexOf(vertexes.at(ii*3 +2).position()));
			}

			CGAL::Polygon_mesh_processing::orient_polygon_soup(points.at(itemIndex), polygons.at(itemIndex));
			Polyhedron mesh;
			CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points.at(itemIndex), polygons.at(itemIndex), mesh);
			// Number the faces because 'orient_to_bound_a_volume' needs a face <--> index map
			int index = 0;
			for(Polyhedron::Face_iterator fb=mesh.facets_begin(), fe=mesh.facets_end(); fb!=fe; ++fb)
			  fb->id() = index++;
			if(CGAL::is_closed(mesh))
			{
				CGAL::Polygon_mesh_processing::orient_to_bound_a_volume(mesh);
			}

			meshes.at(itemIndex) = mesh;
		}
	}

	//for every extruded additive item, subtract every extruded subtractive item
	for(int itemIndex = 0; itemIndex < itemsInSpace->size(); itemIndex++)
	{
		Item *currentItem = itemsInSpace->at(itemIndex);

		//every extruded additive item
		if(currentItem->isExtruded())
		{
			if(currentItem->isAdditive())
			{
				for(uint32_t subtractiveItemIndex = 0; subtractiveItemIndex < meshes.size(); subtractiveItemIndex++)
				{
					Item *insideItem = itemsInSpace->at(subtractiveItemIndex);

					//every extruded subtractive item
					if((!insideItem->isAdditive()) & insideItem->isExtruded())
					{
						bool valid_difference =
						  PMP::corefine_and_compute_difference(meshes.at(itemIndex),
															   meshes.at(subtractiveItemIndex),
															   meshes.at(itemIndex),
															   params::all_default(), // default parameters for mesh1
															   params::all_default() // default parameters for mesh2
															   );

						if(!valid_difference) std::cout << "Warning: subtraction not valid (SpaceFactory:327)";
					}
				}
				CGAL::Polygon_mesh_processing::triangulate_faces(meshes.at(itemIndex));

				//convert via file
				QString filePath = QDir::tempPath() + "/tempmesh.off";
				std::ofstream out(filePath.toStdString());
				out.precision(17);
				out << meshes.at(itemIndex);
				out.close();


				//read file
				QFile tmpFile(filePath);
				tmpFile.open(QFile::OpenModeFlag::ReadOnly);
				QString file = tmpFile.readAll();

			qDebug() << file;

				//load file to vertex list
				QStringList lines = file.split('\n');
				//header - filetype - line 1
				if(lines.at(0).trimmed() == "OFF")
				{
					//header - count - line 0
					int pointCount = lines.at(1).section(' ', 0,0).toInt();
					int faceCount = lines.at(1).section(' ', 1,1).toInt();

					std::vector<QVector3D> points;
					std::vector<Vertex> vertexData;

					const int headerSize = 3;

					//points
					for(int lineNum = headerSize; lineNum < pointCount + headerSize; lineNum++)
					{
						points.push_back(
									QVector3D(
										lines.at(lineNum).section(' ', 0,0).toDouble(),
										lines.at(lineNum).section(' ', 1,1).toDouble(),
										lines.at(lineNum).section(' ', 2,2).toDouble()
										));
					}

					//faces
					for(int lineNum = headerSize + pointCount; lineNum < faceCount + pointCount + headerSize; lineNum++)
					{
						QString debugStr;
						for(int vertexIndex = 1; vertexIndex < 4; vertexIndex++)
						{
							int index = lines.at(lineNum).section(' ', vertexIndex, vertexIndex).toInt();

							debugStr += QString::number(points.at(index).x()) + " ";
							debugStr += QString::number(points.at(index).y()) + " ";
							debugStr += QString::number(points.at(index).z()) + " ";
							vertexData.push_back(Vertex(points.at(index)));
							debugStr += ";		";
						}
						qDebug() << debugStr;
					}
					result.at(itemIndex) = vertexData;
				}
			}
		}
	}

	return result;
}
#endif

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
