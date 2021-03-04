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
	//vertexes = global pos, need to covert to local 2D
	if(vertexes.size() == 0) return;

	for (uint32_t i = 0; i < vertexes.size(); i++)
	{
		projectedPoints << (vertexes.at(i) * toMatrix().inverted()).toPointF() * Settings::planeToSpaceRatio;
	}
	projectedPoints << (vertexes.at(0) * toMatrix().inverted()).toPointF() * Settings::planeToSpaceRatio;
}

QPolygonF Plane::getProjectedPolygon()
{
	return projectedPoints;
}
