#ifndef PLANE_H
#define PLANE_H

#include <QTreeWidgetItem>
#include <QVector3D>

namespace Space {

class Plane : public QTreeWidgetItem
{
public:
	Plane();
	Plane(QTreeWidgetItem *parent, QVector3D position, QVector3D vector);

	QVector3D getPosition() const;
	void setPosition(const QVector3D &value);
	QVector3D getVector() const;
	void setVector(const QVector3D &value);

private:
	QVector3D position;
	QVector3D vector;
};
}

#endif // PLANE_H
