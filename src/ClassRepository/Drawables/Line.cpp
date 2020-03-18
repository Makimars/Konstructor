#include "Line.h"

Line::Line() : DrawableObject (Type_Line){}


Line::Line(Point *startPoint, Point *endPoint) : DrawableObject (Type_Line)
{
	this->startPoint = startPoint;
	this->endPoint = endPoint;
	setGeometryUpdates();
}

//----------	file handling    ----------

void Line::loadVariables(QString input)
{
	fetchVariables(input);
}

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

double Line::getLength()
{
	return this->startPoint->distanceFrom(endPoint->getLocation());
}

Line *Line::setLength(float lenght)
{
	QVector2D newVector = this->getLineVector().normalized() * lenght;

	updateGeometry();
	this->endPoint->setLocation(
				this->startPoint->getX() + newVector.x(),
				this->startPoint->getY() + newVector.y()
				);

	return this;
}

QVector2D Line::getLineVector() const
{
	return QVector2D(
				this->endPoint->getX() - this->startPoint->getX(),
				this->endPoint->getY() - this->startPoint->getY()
				);
}

Line *Line::setLineVector(QVector2D vector)
{
	vector.normalize();
	vector *= this->getLength();
	QVector2D lineVector = this->getLineVector();

	updateGeometry();
	this->endPoint->setLocation(
				this->startPoint->getY() + lineVector.x(),
				this->startPoint->getY() + lineVector.y()
                );

	return this;
}

Point *Line::getStartPoint()
{
	return this->startPoint;
}

Point *Line::getEndPoint()
{
	return this->endPoint;
}

Line *Line::clone()
{
	Line *l = new Line(this->startPoint, this->endPoint);
	l->setName(this->getName());

	return l;
}

//----------     Distance    ----------

double Line::distanceFrom(QPointF location)
{
	return abs(signedDistanceFrom(location));
}

double Line::signedDistanceFrom(QPointF location)
{
	double denominator = (
				((this->endPoint->getY() - this->startPoint->getY()) * location.x()) -
				((this->endPoint->getX() - this->startPoint->getX()) * location.y()) +
				(this->endPoint->getX() * this->startPoint->getY()) -
				(this->endPoint->getY() * this->startPoint->getX())
				);
	double numerator = sqrt(
				pow(this->endPoint->getY() - this->startPoint->getY(), 2) +
				pow(this->endPoint->getX() - this->startPoint->getX(), 2)
				);

	return denominator / numerator;
}

//----------	QGraphicsItem overrides    ----------

QRectF Line::boundingRect() const
{
	return QRectF(this->startPoint->getLocation(), this->endPoint->getLocation());
}

QPainterPath Line::shape() const
{
	QVector2D lineVector = this->getLineVector().normalized();
	QVector2D normalVector(
				-lineVector.y(),
				lineVector.x()
						);
	normalVector *= 4;
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
	if(this->isHidden())
		return;

	DrawableObject::paint(painter, option, widget);

	painter->drawLine(this->startPoint->getLocation(),
						this->endPoint->getLocation()
					  );
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


