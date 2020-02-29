#include "LinesAngleDimension.h"

LinesAngleDimension::LinesAngleDimension(Line *lines[])
{
	this->type = TYPE_LINE_ANGLE_DIMENSION;

	this->lines[0] = lines[0];
	this->lines[1] = lines[1];

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

	this->angle = lines[0]->getAngle(lines[1]->getLineVector());
}

void LinesAngleDimension::resolveTies()
{
	this->lines[0]->setAngle(this->angle, this->lines[1]->getLineVector());
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

//----------    Getters and setters    -----------

void LinesAngleDimension::setDistanceFromCenter(double distance)
{
	this->distanceFromCenter = distance;
}

//----------	QGraphicsItem overrides    ----------

QRectF LinesAngleDimension::boundingRect() const
{
	double x,y;

	if(this->edgePoints[0]->getX() > this->edgePoints[1]->getX())
		x = this->edgePoints[0]->getX();
	else
		x = this->edgePoints[1]->getX();

	if(this->edgePoints[0]->getY() > this->edgePoints[1]->getY())
		y = this->edgePoints[0]->getY();
	else
		y = this->edgePoints[1]->getY();

	double distance = this->edgePoints[0]->distanceFrom(this->edgePoints[1]->getLocation());

	QPointF upperRightPoint(x - distance,y - distance);

	return QRectF(upperRightPoint.x(), upperRightPoint.y(), distance * 3 , distance * 3);
}

QPainterPath LinesAngleDimension::shape() const
{	
	QVector2D lineVector = lines[0]->getLineVector();

	if(this->lines[0]->getEndPoint() == this->commonPoint)
		lineVector *= -1;

	QPointF virtualPoint = this->edgePoints[1]->getLocation() + lineVector.toPointF();
	QPolygonF shape;
	shape << this->commonPoint->getLocation() << this->edgePoints[0]->getLocation() << virtualPoint << this->edgePoints[1]->getLocation() << this->commonPoint->getLocation();
	QPainterPath interPath;
	interPath.addPolygon(shape);

	QRectF baseRect(
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

	QPainterPath outerCircle;
	outerCircle.addEllipse(this->commonPoint->getLocation(),this->distanceFromCenter, this->distanceFromCenter);

	QPainterPath innerCircle;
	innerCircle.addEllipse(this->commonPoint->getLocation(),this->distanceFromCenter - 1, this->distanceFromCenter - 1);

	painter->drawPath(outerCircle.subtracted(innerCircle).intersected(shape()));
	painter->drawText(this->edgePoints[0]->getLocation(),QString::number(this->angle));
}

//---------     events     ----------

void LinesAngleDimension::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->draging)
	{
		double shorterLineLength;
		if(this->lines[0]->getLength() < this->lines[1]->getLength())
			shorterLineLength = this->lines[0]->getLength();
		else
			shorterLineLength = this->lines[1]->getLength();

		double newDistance = this->commonPoint->distanceFrom(event->pos());
		if(newDistance < shorterLineLength)
			this->distanceFromCenter = newDistance;
	}
}

void LinesAngleDimension::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	emit requestDouble(this);
}

//----------     user input requests      ---------

void LinesAngleDimension::recieveDouble(double value)
{
	this->angle = value;
}
