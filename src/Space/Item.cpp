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

void Item::setPolygons(std::vector<QPolygonF> polygons)
{
	this->polygons.clear();
	foreach(QPolygonF polygon, polygons)
	{
		this->polygons.push_back(new Polygon(polygon));
	}

	emit updateData();
}

QVector<Polygon*> *Item::getPolygons()
{
	return &polygons;
}

void Item::setItemIndex(int index)
{
	itemIndex = index;
}

void Item::setDataSize(int size)
{
	extrudedDataSize = size;
}

void Item::setSketch(QString sketch)
{
	this->sketch = sketch;
}

int Item::getDataSize()
{
	if(extruded)
	{
		return extrudedDataSize;
	}
	else
	{
		int size = 0;
		for(int i = 0; i < polygons.size(); i++)
		{
			size += polygons.at(i)->getDataSize();
		}

		return size;
	}
}

bool Item::isExtruded()
{
	return extruded;
}

QVector3D Item::getColor()
{
	if(extruded)return extrudedPolygon->getColor();

	return QVector3D(0,0,0);
}

Plane *Item::getPlane(int index)
{
	return planes.at(index);
}

void Item::extrude()
{
	std::vector<QPointF> polygonPoints = extrudedPolygon->getPoints();
	for (u_int32_t i = 0; i < polygonPoints.size(); i++)
	{
		extrudedVertexes.push_back(Vertex(
						polygonPoints.at(i).x(),
						polygonPoints.at(i).y(),
						0
					));
	}

	//calculate length
	double length = extrusion.length;

	if(extrusion.direction == ExtrusionDirection::Back)
	{
		length = -length;
	}
	else if (extrusion.direction == ExtrusionDirection::FrontAndBack)
	{
		length *= 0.5;

		for (uint32_t i = 0; i < extrudedVertexes.size(); i++)
		{
			extrudedVertexes.at(i).setZ(-length);
		}
		addPlane(0, basePlane->getPosition() + QVector3D(0,0,-length), basePlane->getRotation());

	}

	//calculate second base
	int count = extrudedVertexes.size();
	for (int i = 0; i < count; i++)
	{
		Vertex vertex = extrudedVertexes.at(i);
		vertex.setPosition(QVector3D(vertex.position().x(), vertex.position().y(), length));

		extrudedVertexes.push_back(vertex);
	}
	addPlane(1, basePlane->getPosition() + QVector3D(0,0,length), basePlane->getRotation());

	//TODO calculate planes for faces

	emit updateData();
	setExpanded(true);
}

void Item::extrude(Extrusion extrusion, Polygon *targetPolygon)
{
	extruded = true;
	this->extrudedPolygon = targetPolygon;
	this->extrusion = extrusion;

	extrude();
}

std::vector<Vertex> *Item::getExtrudedVertexes()
{
	return &extrudedVertexes;
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
