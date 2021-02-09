#include "Plane.h"

Plane::Plane()
{
	this->setIcon(0, QIcon(":/icons/Plane.png"));
	setText(0, "Plane");
	setCheckState(0,Qt::CheckState::Unchecked);
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

void Plane::setExistingVertexes(std::vector<QVector3D> vertexes)
{
	for (uint32_t i = 0; i < vertexes.size(); i++)
	{
		projectedVertexes.push_back(
					(vertexes.at(i) * toMatrix().inverted()).toPointF() * Settings::planeToSpaceRatio
					);
	}
}

QPolygonF Plane::getProjectedPolygon()
{
	QPolygonF polygon;
	if(projectedVertexes.size() == 0) return polygon;

	for (uint32_t i = 0; i < projectedVertexes.size(); i++)
	{
		polygon << projectedVertexes.at(i);
	}
	polygon << projectedVertexes.at(0);

	return polygon;
}
