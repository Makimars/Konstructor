#include "Item.h"

Item::Item()
{
	this->setIcon(0, QIcon(":/icons/Cube.png"));
}

Item::Item(Plane *plane, std::vector<QPolygonF> polygons, QString sketch) : Item()
{
	this->sketch = sketch;
	this->basePlane = plane;

	setPolygons(polygons);

	plane->addChild(this);
	plane->setExpanded(true);
}

Item::~Item()
{
	for (uint32_t i = 0; i < planes.size(); i++ )
	{
		emit deletePlane(planes.at(i));
	}
}

void Item::copyVertexesToReference(std::vector<Vertex*> vector, int itemIndex)
{
	targetVertexBuffer = vector;
		this->itemIndex = itemIndex;

		if(extruded)
		{
			for(uint32_t i = 0; i < vertexBuffer.size(); i++)
			{
				*targetVertexBuffer.at(i) = vertexBuffer.at(i);
			}
		}
		else
		{
			//copy polygons to global vertex
			for(int i = 0; i < polygons.size(); i++)
			{
				for(int a = 0; a < polygons.at(i)->size(); a++)
				{
					*targetVertexBuffer.at(i+a) = polygons.at(i)->getVertexAt(a);
				}
			}
		}
}

void Item::setPolygons(std::vector<QPolygonF> polygons)
{
	foreach(QPolygonF polygon, polygons)
	{
		this->polygons.push_back(new Polygon(polygon));
	}

	this->targetVertexBuffer.reserve(size());
	emit updateData();
}

QVector<Polygon*> *Item::getPolygons()
{
	return &polygons;
}

void Item::setSketch(QString sketch)
{
	this->sketch = sketch;
}

int Item::size()
{
	if(extruded)
	{
		return vertexBuffer.size();
	}
	else
	{
		int size = 0;
		for(int i = 0; i < polygons.size(); i++)
		{
			size += polygons.at(i)->size();
		}

		return size;
	}
}

bool Item::isExtruded()
{
	return extruded;
}

Plane *Item::getPlane(int index)
{
	return planes.at(index);
}

void Item::extrude()
{
	//copy polygon vertexes
	vertexBuffer = *extrudedPolygon->getVertexData();
	std::vector<Vertex> originalEdges = *extrudedPolygon->getOuterPoints();

	//calculate length
	double length = extrusion.length;

	if(extrusion.direction == ExtrusionDirection::Back)
	{
		length = -length;
	}
	else if (extrusion.direction == ExtrusionDirection::FrontAndBack)
	{
		length *= 0.5;

		for (uint32_t i = 0; i < vertexBuffer.size(); i++)
		{
			vertexBuffer.at(i).setZ(-length);
		}
		for (uint32_t i = 0; i < originalEdges.size(); i++)
		{
			originalEdges.at(i).setZ(-length);
		}
		addPlane(0, basePlane->getPosition() + QVector3D(0,0,-length), basePlane->getRotation());

	}

	//calculate second base
	int count = vertexBuffer.size();
	for (int i = 0; i < count; i++)
	{
		Vertex vertex = vertexBuffer.at(i);
		vertex.setPosition(QVector3D(vertex.position().x(), vertex.position().y(), length));

		vertexBuffer.push_back(vertex);
	}
	addPlane(1, basePlane->getPosition() + QVector3D(0,0,length), basePlane->getRotation());

	//calculate faces
	for (uint32_t i = 0; i < originalEdges.size() - 1; i++)
	{
		Vertex copyVertex;

		vertexBuffer.push_back(originalEdges.at(i));
		vertexBuffer.push_back(originalEdges.at(i + 1));
		copyVertex = originalEdges.at(i);
		copyVertex.setZ(length);
		vertexBuffer.push_back(copyVertex);

		copyVertex = originalEdges.at(i);
		copyVertex.setZ(length);
		vertexBuffer.push_back(copyVertex);
		vertexBuffer.push_back(originalEdges.at(i + 1));
		copyVertex = originalEdges.at(i + 1);
		copyVertex.setZ(length);
		vertexBuffer.push_back(copyVertex);
	}
	//calculate closing faces
	{
		Vertex copyVertex;
		int lastOuterIndex = originalEdges.size() - 1;

		vertexBuffer.push_back(originalEdges.at(lastOuterIndex));
		vertexBuffer.push_back(originalEdges.at(0));
		copyVertex = originalEdges.at(lastOuterIndex);
		copyVertex.setZ(length);
		vertexBuffer.push_back(copyVertex);

		copyVertex = originalEdges.at(lastOuterIndex);
		copyVertex.setZ(length);
		vertexBuffer.push_back(copyVertex);
		vertexBuffer.push_back(originalEdges.at(0));
		copyVertex = originalEdges.at(0);
		copyVertex.setZ(length);
		vertexBuffer.push_back(copyVertex);
	}
	//TODO add planes for faces

	updateData();
	setExpanded(true);
}

void Item::extrude(Extrusion extrusion, Polygon *targetPolygon)
{
	extruded = true;
	this->extrudedPolygon = targetPolygon;
	this->extrusion = extrusion;

	extrude();
}

nlohmann::json Item::toJson()
{
	nlohmann::json file;

	file["sketch"] = this->sketch.toStdString();

	if(extruded == true)
	{

		file["extrusionPolygon"] = polygons.indexOf(extrudedPolygon);

		file["extrusion"]["length"] = extrusion.length;
		file["extrusion"]["additive"] = extrusion.additive;
		file["extrusion"]["direction"] = extrusion.direction;
	}
	else
	{
		file["extrusionPolygon"] = -1;
	}

	file["basePlane"] = basePlane->getId().toStdString();

	return file;
}

void Item::loadData(QString basePlaneId, Extrusion extrusion, bool extruded, int extrudedPolygon)
{
	this->basePlaneId = basePlaneId;
	this->extrusion = extrusion;
	this->extruded = extruded;

	this->extrudedPolygon = polygons.at(extrudedPolygon);
}

void Item::loadRelations(std::vector<Item*> list)
{
	QStringList Ids = basePlaneId.split(":");
	int itemId = QVariant(Ids[0]).toInt();
	int planeId = QVariant(Ids[1]).toInt();

	if(itemId == -1)
	{
		basePlane = emit getBasePlane();
	}
	else
	{
		basePlane = list.at(itemId)->getPlane(planeId);
	}

	basePlane->addChild(this);
}

void Item::addPlane(int index, QVector3D position, QQuaternion rotation)
{
	// ensures that the vector has either nullptr or an instance
	if(planes.size() < index + 1)
	{
		for (int i = planes.size(); i <= index; i++)
		{
			planes.push_back(nullptr);
		}
	}

	//if an index is nullptr, create a new plane. if not, update
	if(planes.at(index) == nullptr)
	{
		Plane *newPlane = new Plane(this, position, rotation);
		newPlane->setText(0, tr("plane"));

		//adds to local vector
		planes.at(index) = newPlane;
		//adds to tree
		addChild(newPlane);
		//adds to buffer (view)
		emit planeAdded(newPlane);

		newPlane->setPlaneIndex(index);
	}
	else
	{
		//rewrite plane
		*planes.at(index) =  Plane(this, position, rotation);
	}
}
