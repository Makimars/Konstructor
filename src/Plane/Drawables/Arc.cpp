#include "Arc.h"

Arc::Arc() : DrawableObject(Global::Arc){}

Arc::Arc(Point *leftPoint, Point *rightPoint, Point *centrePoint) : DrawableObject(Global::Arc)
{
	this->centerPoint = centrePoint;
	edgePoints[0] = leftPoint;
	edgePoints[1] = rightPoint;

	resolveTies();
	radius = this->centerPoint->distanceFrom(this->edgePoints[0]->getLocation());
}

void Arc::resolveTies()
{
	//line vector of line between edge points
	QVector2D lineVector(edgePoints[0]->getLocation() - edgePoints[1]->getLocation());
	lineVector.normalize();
	lineVector = QVector2D(lineVector.y(), -lineVector.x());

	//centre point
	double deltaX = (edgePoints[0]->getX() + edgePoints[1]->getX()) / 2;
	double deltaY = (edgePoints[0]->getY() + edgePoints[1]->getY()) / 2;
	QPointF acrossPoint(deltaX, deltaY);

	double centerDelta = centerPoint->distanceFrom(acrossPoint);

	radius = this->centerPoint->distanceFrom(this->edgePoints[0]->getLocation());
	centerPoint->setLocation(acrossPoint + (lineVector*centerDelta).toPointF());
}

void Arc::loadVariables(QString input){}

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

double Arc::getRadius() const
{
	return radius;
}

QRectF Arc::boundingRect() const
{
	return shape().boundingRect();
}

QPainterPath Arc::shape() const
{
	//line vector of line between edge points
	QVector2D lineVector(edgePoints[0]->getLocation() - edgePoints[1]->getLocation());
	lineVector.normalize();
	lineVector = QVector2D(-lineVector.y(), lineVector.x());

	double edgeDistance = edgePoints[0]->distanceFrom(edgePoints[1]->getLocation());

	QPointF upperLeft = edgePoints[0]->getLocation() + (lineVector * edgeDistance/2).toPointF();
	QPointF upperRight = edgePoints[1]->getLocation() + (lineVector * edgeDistance/2).toPointF();

	QPolygonF polygon;
	polygon << this->centerPoint->getLocation()
			<< this->edgePoints[0]->getLocation()
			<< upperLeft
			<< upperRight
			<< this->edgePoints[1]->getLocation()
			<< centerPoint->getLocation();

	QPainterPath path;
	path.addPolygon(polygon);

	return path;
}

void Arc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(isHidden()) return;

	DrawableObject::paint(painter);

	double radius = getRadius();
	QPainterPath drawPath;
	drawPath.addEllipse(this->centerPoint->getLocation(), radius, radius);

	QPainterPath subtractPath;
	subtractPath.addEllipse(this->centerPoint->getLocation(), radius-0.5, radius-0.5);

	painter->drawPath(drawPath.intersected(shape()));
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
