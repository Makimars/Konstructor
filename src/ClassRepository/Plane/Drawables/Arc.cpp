#include "Arc.h"

Arc::Arc() : DrawableObject(Global::Arc)
{

}

Arc::Arc(Point *leftPoint, Point *rightPoint, Point *centrePoint) : DrawableObject(Global::Arc)
{
	this->centerPoint = centrePoint;
	edgePoints[0] = leftPoint;
	edgePoints[1] = rightPoint;
}

void Arc::resolveTies()
{

}

void Arc::loadVariables(QString input)
{

}

QString Arc::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("centerPoint", this->centerPoint->getId());
	this->fileAddVar("leftPoint", this->edgePoints[0]->getId());
	this->fileAddVar("rightPoint", this->edgePoints[1]->getId());
	return this->fileFinish();
}

void Arc::loadRelations(QVector<DrawableObject*> list)
{
	QStringList varNames = {
		"centerPoint",
		"leftPoint",
		"rightPoint"
	};

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->centerPoint = dynamic_cast<Point*>(values[0]);
	this->edgePoints[0] = dynamic_cast<Point*>(values[1]);
	this->edgePoints[1] = dynamic_cast<Point*>(values[2]);
	setGeometryUpdates();
}

double Arc::getRadius()
{

}

QRectF Arc::boundingRect() const
{

}

QPainterPath Arc::shape() const
{

}

void Arc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(isHidden()) return;

	DrawableObject::paint(painter);

	painter->drawEllipse(centerPoint->getLocation(), getRadius(), getRadius());
}

void Arc::setGeometryUpdates()
{
	this->centerPoint->addGeometryUpdate(this);
	edgePoints[0]->addGeometryUpdate(this);
	edgePoints[1]->addGeometryUpdate(this);
}

void Arc::unsetGeometryUpdates()
{
	this->centerPoint->removeGeometryUpdate(this);
	edgePoints[0]->removeGeometryUpdate(this);
	edgePoints[0]->removeGeometryUpdate(this);
}
