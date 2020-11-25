#include "Item.h"

Item::Item(Space::Plane *plane, std::vector<QPolygonF> polygons, QString sketch)
{
	this->sketch = sketch;
	this->basePlane = plane;

	setPolygons(polygons);

	this->setIcon(0, QIcon(":/icons/Cube.png"));
	plane->addChild(this);
	plane->setExpanded(true);
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
			for(uint32_t i = 0; i < polygons.size(); i++)
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

std::vector<Polygon*> *Item::getPolygons()
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
		for(uint32_t i = 0; i < polygons.size(); i++)
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

void Item::extrude(Extrusion extrusion, Polygon *targetPolygon)
{
	extruded = true;
	this->extrudedPolygon = targetPolygon;
	this->extrusion = extrusion;

	//copy polygon vertexes
	vertexBuffer = *targetPolygon->getVertexData();
	std::vector<Vertex> originalEdges = *targetPolygon->getOuterPoints();

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
	}

	//calculate second base
	int count = vertexBuffer.size();
	for (int i = 0; i < count; i++)
	{
		Vertex vertex = vertexBuffer.at(i);
		vertex.setPosition(QVector3D(vertex.position().x(), vertex.position().y(), length));

		vertexBuffer.push_back(vertex);
	}
	addPlane(this, basePlane->getPosition() + QVector3D(0,0,0), basePlane->getRotation());

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
}

void Item::addPlane(QTreeWidgetItem *parent, QVector3D position, QQuaternion rotation)
{
	Space::Plane *newPlane = new Space::Plane(parent, position, rotation);
	addChild(newPlane);
	emit planeAdded(newPlane);
}
