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

void Item::setPolygons(std::vector<QPolygonF> polygons)
{
	foreach(QPolygonF polygon, polygons)
	{
		Polygon *finalPolygon = new Polygon(polygon);

		connect(finalPolygon, &Polygon::updateData,
				this, &Item::updateData
				);

		this->polygons.push_back(finalPolygon);
	}

	this->vertexes.reserve(size());
	emit updateData();
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
