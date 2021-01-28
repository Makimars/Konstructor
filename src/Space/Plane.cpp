#include "Plane.h"

Plane::Plane()
{
	this->setIcon(0, QIcon(":/icons/Plane.png"));
	setCheckState(0,Qt::CheckState::Checked);
	itemIndex = -1;
	planeIndex = 0;
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

QString Plane::getId()
{
	return QString::number(itemIndex) + ":" + QString::number(planeIndex);
}

void Plane::setItemIndex(int i)
{
	itemIndex = i;
}

void Plane::setPlaneIndex(int i)
{
	planeIndex = i;
}
