#include "Item.h"

Item::Item(QVector<DrawableObject *> sketchObjects, QPointF planePosition, QVector3D planeVector)
{
	this->sketchObjects = sketchObjects;
	generateVertexes();
}

void Item::generateVertexes()
{

}
