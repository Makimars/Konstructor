#include "LinesAngleDimension.h"

LinesAngleDimension::LinesAngleDimension(Line *lines[], double angle)
{
	this->type = TYPE_LINE_ANGLE_DIMENSION;

	this->lines[0] = lines[0];
	this->lines[1] = lines[1];
	this->angle = angle;

	if(this->lines[0]->getStartPoint() == this->lines[1]->getStartPoint()	|
		this->lines[0]->getStartPoint() == this->lines[1]->getEndPoint()	)
	{
		this->commonPoint = this->lines[0]->getStartPoint();

		this->edgePoints[0] = this->lines[0]->getEndPoint();

		if(commonPoint == this->lines[1]->getStartPoint())
			this->edgePoints[1] = lines[1]->getEndPoint();
		else
			this->edgePoints[1] = lines[1]->getStartPoint();
	}
	else
	{
		this->commonPoint = this->lines[0]->getEndPoint();

		this->edgePoints[0] = this->lines[0]->getStartPoint();

		if(commonPoint == this->lines[1]->getStartPoint())
			this->edgePoints[1] = lines[1]->getEndPoint();
		else
			this->edgePoints[1] = lines[1]->getStartPoint();
	}
	qDebug() << "construct";
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
	QVector2D lineVector = lines[0]->getLineVector();

	if(this->lines[0]->getEndPoint() == this->commonPoint)
		lineVector *= -1;

	QPointF virtualPoint = this->edgePoints[1]->getLocation() + lineVector.toPointF();
	QPolygonF shape;
	shape << this->commonPoint->getLocation() << this->edgePoints[0]->getLocation() << virtualPoint << this->edgePoints[1]->getLocation();
	QPainterPath interPath;
	interPath.addPolygon(shape);

	QRect baseRect(
				this->commonPoint->getX() - this->distanceFromCenter,
				this->commonPoint->getY() - this->distanceFromCenter,
				this->distanceFromCenter * 2,
				this->distanceFromCenter * 2
				);

	QPainterPath innerEllipse;
	innerEllipse.addEllipse(baseRect - Settings::lineAngleArcMargins);

	QPainterPath outerEllipse;
	outerEllipse.addEllipse(baseRect + Settings::lineAngleArcMargins);

	return outerEllipse.subtracted(innerEllipse).intersected(interPath);
}

void LinesAngleDimension::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(this->hidden)
		return

	DrawableObject::paint(painter, option, widget);
	painter->setPen(*this->pen);

	QPainterPath circle;
	circle.addEllipse(this->commonPoint->getLocation(),this->distanceFromCenter, this->distanceFromCenter);

	QPolygonF anglePolygon;

	anglePolygon << this->commonPoint->getLocation();
	anglePolygon << this->edgePoints[0]->getLocation() << this->edgePoints[1]->getLocation();
	anglePolygon << this->commonPoint->getLocation();

	QPainterPath allowed;
	allowed.addPolygon(anglePolygon);

	//painter->drawPath(circle.intersected(allowed));
	painter->drawPath(circle.intersected(shape()));

	//debug
painter->drawPath(shape());
}

//---------     events     ----------

void LinesAngleDimension::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << "lineangle mouse move";

	if(this->draging)
		this->distanceFromCenter = this->commonPoint->distanceFrom(event->pos());
}

void LinesAngleDimension::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}
