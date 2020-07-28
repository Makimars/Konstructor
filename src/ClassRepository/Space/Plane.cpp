#include "Plane.h"

using namespace Space;

Plane::Plane()
{
	this->setIcon(0, QIcon(":/icons/Plane.png"));
}

Plane::Plane(QTreeWidgetItem *parent, QVector3D position, QQuaternion rotation) : Plane()
{
	parent->addChild(this);
	transform.setTranslation(position);
	transform.setRotation(rotation);
}

QVector3D Plane::getPosition() const
{
	return transform.translation();
}

void Plane::setPosition(const QVector3D &value)
{
	transform.setTranslation(value);
}

QQuaternion Plane::getRotation() const
{
	return transform.rotation();
}

void Plane::setRotation(const QQuaternion &rotation)
{
	transform.setRotation(rotation);
}
