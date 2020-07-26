#include "Item.h"

Item::Item(Space::Plane *plane, QString sketch)
{
	sketch = sketch;

	this->basePlane = plane;

	this->setIcon(0, QIcon(":/icons/Cube.png"));
	plane->addChild(this);
	plane->setExpanded(true);
}

void Item::setVectorReference(std::vector<Vertex*> vector, int itemIndex)
{
	vertexes = vector;
	this->itemIndex = itemIndex;

	//copy the original data into the global buffer
	for(int i = 0; i < vertexData.size(); i++)
		*vertexes[i] = vertexData[i];
}

void Item::setPlaneVertexes(std::vector<Vertex> vertexes)
{
	vertexData.clear();
	vertexData = pointsToSpaceVertexes(vertexes);
	this->vertexes.reserve(size());
    emit sizeChanged();
}

void Item::extrude(double length, bool extrusion, ExtrusionDirection direction)
{
qDebug() << length;
qDebug() << extrusion;
}

std::vector<Vertex> Item::pointsToSpaceVertexes(std::vector<Vertex> planeVertexes)
{
	//generate Space vertexes from points

	//multiply by plane vector

	//add plane base
	foreach(Vertex vertex, planeVertexes)
	{
		QVector3D pos = vertex.position();
		vertex.setPosition(pos + basePlane->getPosition());
	}

	return planeVertexes;
}
