#ifndef PLANE_H
#define PLANE_H

#include <QTreeWidgetItem>
#include <QVector3D>

#include "Transform3D.h"
#include "src/Base/Settings.h"

class Plane : public QTreeWidgetItem
{
public:
	Plane();
	Plane(QTreeWidgetItem *parent, QVector3D position, QQuaternion rotation);

	QVector3D getPosition() const;
	void setPosition(const QVector3D &value);
	QQuaternion getRotation() const;
	void setRotation(const QQuaternion &rotation);
	QString getId();
	void setItemIndex(int i);
	void setPlaneIndex(int i);

	/**
	 * @brief sets vertexes from which the plane is defined
	 * @param vertexes (global pos)
	 */
	void setExistingVertexes(std::vector<QVector3D> vertexes);
	QPolygonF getProjectedPolygon();

	QMatrix4x4 toMatrix();

private:
	Transform3D transform;
	int itemIndex, planeIndex;
	std::vector<QPointF> projectedVertexes;
};

inline QMatrix4x4 Plane::toMatrix(){ return transform.toMatrix(); }

#endif // PLANE_H
