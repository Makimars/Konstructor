#include "Line.h"

Line::Line() : DrawableObject (Global::Line){}


Line::Line(Point *startPoint, Point *endPoint) : DrawableObject (Global::Line)
{
	this->startPoint = startPoint;
	this->endPoint = endPoint;
	setGeometryUpdates();
}

//----------	file handling    ----------

QString Line::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("startPoint", this->startPoint->getId());
	this->fileAddVar("endPoint", this->endPoint->getId());
    return DrawableObject::fileFinish();
}

void Line::loadRelations(QVector<DrawableObject*> list)
{
    QStringList varNames = {
		"startPoint",
		"endPoint"
    };

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->startPoint = dynamic_cast<Point*>(values[0]);
	this->endPoint = dynamic_cast<Point*>(values[1]);
	setGeometryUpdates();
}

//----------	getters and setters    ----------

QVector2D Line::getLineVector() const
{
	return QVector2D(
				this->endPoint->getX() - this->startPoint->getX(),
				this->endPoint->getY() - this->startPoint->getY()
				);
}

Point *Line::getStartPoint()
{
	return this->startPoint;
}

Point *Line::getEndPoint()
{
	return this->endPoint;
}

QPointF Line::getCenterPos()
{
	double x = (startPoint->getLocation().x() + endPoint->getLocation().x()) /2;
	double y = (startPoint->getLocation().y() + endPoint->getLocation().y()) /2;

	return QPointF(x, y);
}

//----------     Distance    ----------

double Line::distanceFrom(QPointF location)
{
	return Line::distanceFrom(startPoint->getLocation(), endPoint->getLocation(), location);
}

double Line::signedDistanceFrom(QPointF location)
{
	return Line::signedDistanceFrom(startPoint->getLocation(), endPoint->getLocation(), location);
}

//----------	QGraphicsItem overrides    ----------

QRectF Line::boundingRect() const
{
	QVector2D lineVector = this->getLineVector().normalized();
	QVector2D normalVector(
				-lineVector.y(),
				lineVector.x()
						);
	normalVector *= Settings::lineShapeSize;
	QPointF startPointOne(this->startPoint->getLocation()+normalVector.toPointF());
	QPointF startPointTwo(this->startPoint->getLocation()-normalVector.toPointF());

	QPointF endPointOne(this->endPoint->getLocation()+normalVector.toPointF());
	QPointF endPointTwo(this->endPoint->getLocation()-normalVector.toPointF());

	std::vector<qreal> x = {startPointOne.x(), startPointTwo.x(), endPointOne.x(), endPointTwo.x()};
	std::sort(x.begin(), x.end());

	std::vector<qreal> y = {startPointOne.y(), startPointTwo.y(), endPointOne.y(), endPointTwo.y()};
	std::sort(y.begin(), y.end());

	return QRectF(QPointF(x.front(), y.front()), QPointF(x.back(), y.back()));
}

QPainterPath Line::shape() const
{
	QVector2D lineVector = this->getLineVector().normalized();
	QVector2D normalVector(
				-lineVector.y(),
				lineVector.x()
						);
	normalVector *= Settings::lineShapeSize;
	QPointF startPointOne(this->startPoint->getLocation()+normalVector.toPointF());
	QPointF startPointTwo(this->startPoint->getLocation()-normalVector.toPointF());

	QPointF endPointOne(this->endPoint->getLocation()+normalVector.toPointF());
	QPointF endPointTwo(this->endPoint->getLocation()-normalVector.toPointF());

	QPolygonF polygon;
	polygon << startPointOne << startPointTwo << endPointTwo << endPointOne << startPointOne;

	QPainterPath path;
	path.addPolygon(polygon);

	return path;
}

void Line::paint(QPainter *painter,
				 const QStyleOptionGraphicsItem *option,
				 QWidget *widget)
{
	if(this->isHidden())return;

	DrawableObject::paint(painter);

	painter->drawLine(this->startPoint->getLocation(),
						this->endPoint->getLocation()
					  );
}

double Line::distanceFrom(QPointF p0, QPointF p1, QPointF location)
{
	return abs((long int)Line::signedDistanceFrom(p0, p1, location));
}

double Line::signedDistanceFrom(QPointF p0, QPointF p1, QPointF location)
{
	double denominator = (
				((p1.y() - p0.y()) * location.x()) -
				((p1.x() - p0.x()) * location.y()) +
				(p1.x() * p0.y()) -
				(p1.y() * p0.x())
				);
	double numerator = sqrt(
				pow(p1.y() - p0.y(), 2) +
				pow(p1.x() - p0.x(), 2)
				);

	return denominator / numerator;
}

//----------     geometry     ----------´

void Line::setGeometryUpdates()
{
	this->startPoint->addGeometryUpdate(this);
	this->endPoint->addGeometryUpdate(this);
}

void Line::unsetGeometryUpdates()
{
	this->startPoint->removeGeometryUpdate(this);
	this->endPoint->removeGeometryUpdate(this);
}


