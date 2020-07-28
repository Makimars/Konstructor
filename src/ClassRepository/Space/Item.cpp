#include "Item.h"

Item::Item(Space::Plane *plane, QString sketch)
{
	this->sketch = sketch;

	this->basePlane = plane;

	this->setIcon(0, QIcon(":/icons/Cube.png"));
	plane->addChild(this);
	plane->setExpanded(true);
}

void Item::setVectorReference(std::vector<Vertex*> vector, int itemIndex)
{
	vertexes = vector;
	this->itemIndex = itemIndex;

	std::vector<Vertex> vertexData;
	for(uint32_t i = 0; i < polygons.size(); i++)
	{
		vertexData.insert(
					vertexData.end(),
					polygons.at(i)->getVertexData()->begin(),
					polygons.at(i)->getVertexData()->end()
					);
	}

	//copy the original data into the global buffer
	for(uint32_t i = 0; i < vertexData.size(); i++)
		*vertexes[i] = vertexData[i];
}

void Item::setPolygons(std::vector<Polygon*> polygons)
{
	this->polygons = polygons;
	for(uint32_t i = 0; i < polygons.size(); i++)
	{
		connect(polygons.at(i), &Polygon::sizeChanged,
				this, &Item::sizeChanged
					);
	}
	this->vertexes.reserve(size());
	emit sizeChanged();
}

std::vector<Polygon*> *Item::getPolygons()
{
	return &polygons;
}

int Item::size()
{
	int size = 0;
	for(uint32_t i = 0; i < polygons.size(); i++)
	{
		size += polygons.at(i)->getVertexData()->size();
	}

	return size;
}
