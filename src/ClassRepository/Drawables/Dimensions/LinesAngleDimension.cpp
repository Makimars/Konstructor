#include "LinesAngleDimension.h"

LinesAngleDimension::LinesAngleDimension(Line *lines[], double angle)
{
	this->lines[0] = lines[0];
	this->lines[1] = lines[1];
	this->angle = angle;

	if(this->lines[0]->getStartPoint() == this->lines[1]->getStartPoint()	|
		this->lines[0]->getStartPoint() == this->lines[1]->getEndPoint()	)
		this->commonPoint = this->lines[0]->getStartPoint();
	else
		this->commonPoint = this->lines[0]->getEndPoint();

}

void LinesAngleDimension::resolveTies()
{

}

void LinesAngleDimension::setValue(double angle)
{
	this->angle = angle;
}

//----------     file handling     ---------

void LinesAngleDimension::fromFileString(QString json)
{

}

QString LinesAngleDimension::toFileString()
{

}

void LinesAngleDimension::loadRelations(QVector<DrawableObject *> *list)
{

}

void LinesAngleDimension::setDistanceFromCenter(float distance)
{
	this->distanceFromCenter = distance;
}

//----------     user input requests      ---------

void LinesAngleDimension::recieveDouble(double value)
{
	this->angle = value;
}

//----------	QGraphicsItem overrides    ----------

QRectF LinesAngleDimension::boundingRect() const
{

	QVector2D lineVector0 = lines[0]->getLineVector().normalized() * this->distanceFromCenter;
	QVector2D lineVector1 = lines[1]->getLineVector().normalized() * this->distanceFromCenter;

	QPointF beginPoint(
				this->commonPoint->getX() + lineVector0.x(),
				this->commonPoint->getY() + lineVector0.y()
						);
	QPointF endPoint(
				this->commonPoint->getX() - lineVector1.x(),
				this->commonPoint->getY() - lineVector1.y()
						);

	return QRectF(beginPoint, endPoint);
}

QPainterPath LinesAngleDimension::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());

	return path;
}

void LinesAngleDimension::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(this->hidden)
		return;

	DrawableObject::paint(painter, option, widget);

	QVector2D lineVector0 = lines[0]->getLineVector().normalized() * this->distanceFromCenter;
	QVector2D lineVector1 = lines[1]->getLineVector().normalized() * this->distanceFromCenter;

	QPointF beginPoint(
				this->commonPoint->getX() + lineVector0.x(),
				this->commonPoint->getY() + lineVector0.y()
						);
	QPointF endPoint(
				this->commonPoint->getX() - lineVector1.x(),
				this->commonPoint->getY() - lineVector1.y()
						);

	QRectF rect(beginPoint, endPoint);
	painter->drawLine(beginPoint,endPoint);
}

//---------     events     ----------

void LinesAngleDimension::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->draging)
		this->distanceFromCenter = this->commonPoint->distanceFrom(event->pos());
}

void LinesAngleDimension::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}
