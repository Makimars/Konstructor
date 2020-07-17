#include "Plane.h"

using namespace Space;

Plane::Plane()
{
	this->setIcon(0, QIcon(":/icons/Plane.png"));
}

Plane::Plane(QTreeWidgetItem *parent, QVector3D position, QVector3D vector) : Plane()
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
