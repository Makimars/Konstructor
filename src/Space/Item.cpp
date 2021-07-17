#include "Item.h"

Item::Item()
{
	this->setIcon(0, QIcon(":/icons/Cube.png"));
	this->setText(0, "Sketch");
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
		Polygon *newPolygon = new Polygon(polygon);
		newPolygon->setText("Poylgon " + QString::number(this->polygons.size()));
		this->polygons.push_back(newPolygon);
	}

	emit updateData();
}

QVector<Polygon*> *Item::getPolygons()
{
	return &polygons;
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

bool Item::isExtruded() const
{
	return extruded;
}

bool Item::isAdditive() const
{
	return extrusion.additive;
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

Plane *Item::getParentPlane()
{
	return basePlane;
}

void Item::setExtrusion()
{
	setText(0, "Item");
	extrudedVertexes.clear();

	std::vector<QPointF> polygonPoints = extrudedPolygon->getPoints();
	for (uint32_t i = 0; i < polygonPoints.size(); i++)
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
		addPlane(0, (basePlane->getPosition() + QVector3D(0,0,-length))*toMatrix(), basePlane->getRotation());

	}

	//calculate second base
	int count = extrudedVertexes.size();
	std::vector<QVector3D> secondBasePlaneVertexes;
	for (int i = 0; i < count; i++)
	{
		Vertex vertex = extrudedVertexes.at(i);
		vertex.setPosition(QVector3D(vertex.position().x(), vertex.position().y(), length));
		secondBasePlaneVertexes.push_back(globalVertexPos(i));

		extrudedVertexes.push_back(vertex);
	}
	addPlane(1,
			 (basePlane->getPosition() + QVector3D(0,0,length) * toMatrix()),
			 basePlane->getRotation(),
			 secondBasePlaneVertexes
			 );

	//calculate planes for faces
	int edges = extrudedVertexes.size() / 2;
	if(edges < Settings::maxItemSidePlanes)
	{
		for (int i = 0; i < edges - 1 ; i++)
		{
			QVector3D basePos = globalVertexPos(i);

			QVector3D rotAxis = globalVertexPos(i+1) - globalVertexPos(i);
			QQuaternion rot = QQuaternion::fromAxisAndAngle(rotAxis, 270);

			std::vector<QVector3D> projectedPoints;
			projectedPoints.push_back(globalVertexPos(i));
			projectedPoints.push_back(globalVertexPos(i+1));
			projectedPoints.push_back(globalVertexPos(edges + i));
			projectedPoints.push_back(globalVertexPos(edges + i + 1));
;
			addPlane(i+2, basePos, rot);
		}
		//overlapping

		QVector3D basePos = globalVertexPos(edges - 1);
		QVector3D rotAxis = globalVertexPos(0) - globalVertexPos(edges-1);
		QQuaternion rot = QQuaternion::fromAxisAndAngle(rotAxis, 270);

		std::vector<QVector3D> projectedPoints;
		projectedPoints.push_back(globalVertexPos(edges - 1));
		projectedPoints.push_back(globalVertexPos(0));
		projectedPoints.push_back(globalVertexPos(edges - 1 + edges));
		projectedPoints.push_back(globalVertexPos(edges));
		//+1 = edges - 1 + 2
		addPlane(edges+1, basePos, rot);
	}

	emit updateData();
	setExpanded(true);
}

void Item::setExtrusion(Extrusion extrusion, Polygon *targetPolygon)
{
	extruded = true;
	this->extrudedPolygon = targetPolygon;
	this->extrusion = extrusion;

	setExtrusion();
}

void Item::reverseExtrusion()
{
	extruded = false;
}

Extrusion Item::getExtrusion()
{
	return extrusion;
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

	if(extruded) this->extrudedPolygon = polygons.at(extrudedPolygon);
}

void Item::loadRelations(std::vector<Item*> list)
{
	QStringList Ids = basePlaneId.split(":");
	int itemId = QVariant(Ids[0]).toInt();
	int planeId = QVariant(Ids[1]).toInt();

	if(itemId == -1)
	{
		basePlane = emit getOriginalPlane();
	}
	else
	{
		basePlane = list.at(itemId)->getPlane(planeId);
	}

	basePlane->addChild(this);
}

QVector3D Item::globalVertexPos(uint32_t i)
{
	return extrudedVertexes.at(i).position() * toMatrix();
}

void Item::addPlane(int index, QVector3D position, QQuaternion rotation)
{
	std::vector<QVector3D> emptyIterator;
	addPlane(index, position, rotation, emptyIterator);
}

void Item::addPlane(int index, QVector3D position, QQuaternion rotation, std::vector<QVector3D> existingVertexes)
{
	rotation.normalize();

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
		newPlane->setExistingVertexes(existingVertexes);

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
		*planes.at(index) = Plane(this, position, rotation);
		planes.at(index)->setExistingVertexes(existingVertexes);
	}
}
