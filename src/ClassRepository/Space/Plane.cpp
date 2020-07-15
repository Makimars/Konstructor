#include "Plane.h"

using namespace Space;

Plane::Plane()
{

}

Plane::Plane(QTreeWidgetItem *parent, QVector3D position, QVector3D vector)
{
	parent->addChild(this);
	this->position = position;
	this->vector = vector;
}

QVector3D Plane::getPosition() const
{
	return position;
}

void Plane::setPosition(const QVector3D &value)
{
	position = value;
}

QVector3D Plane::getVector() const
{
	return vector;
}

void Plane::setVector(const QVector3D &value)
{
	vector = value;
}
