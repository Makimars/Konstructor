#include "LineLengthDimension.h"

LineLengthDimension::LineLengthDimension(Line *line, double length)
{
	this->attachedLine = line;
	this->lengthToSet = length;
	this->distanceFromLine = 20;
	this->type = TYPE_LINE_LENGTH_DIMENSION;
}

void LineLengthDimension::resolveTies()
{
	if(this->lengthToSet > 0)
		this->attachedLine->setLength(this->lengthToSet);
}

void LineLengthDimension::setValue(double length)
{
	this->lengthToSet = length;
}

void LineLengthDimension::setDistanceFromLine(double distance)
{
	this->distanceFromLine = distance;
}

//--------     user input requests     ---------

void LineLengthDimension::recieveDouble(double value)
{
	this->lengthToSet = value;
	resolveTies();
}

//---------    file handeling     ---------

void LineLengthDimension::fromFileString(QString json)
{

}

QString LineLengthDimension::toFileString()
{

}

void LineLengthDimension::loadRelations(QVector<DrawableObject *> *list)
{

}

//----------	QGraphicsItem overrides    ----------

QRectF LineLengthDimension::boundingRect() const
{
	QVector2D lineVector = this->attachedLine->getLineVector().normalized();

	//get normal vector of line with defined length
	QVector2D normalVector(
				-lineVector.y(),
				lineVector.x()
						);
	normalVector *= this->distanceFromLine;

	//edge points
	QPointF aboveStartPoint(
				this->attachedLine->getStartPoint()->getX() + normalVector.x(),
				this->attachedLine->getStartPoint()->getY() + normalVector.y()
				);
	QPointF aboveEndPoint(
				this->attachedLine->getEndPoint()->getX() + normalVector.x(),
				this->attachedLine->getEndPoint()->getY() + normalVector.y()
				);

	return QRectF(aboveStartPoint, aboveEndPoint);
}

QPainterPath LineLengthDimension::shape() const
{
	QPolygonF polygon;

	QVector2D lineVector = this->attachedLine->getLineVector().normalized();

	//get normal vector of line with defined length
	QVector2D normalVector(
				-lineVector.y(),
				lineVector.x()
						);
	normalVector *= this->distanceFromLine;

	//edge points
	QPointF aboveStartPoint(
				this->attachedLine->getStartPoint()->getX() + normalVector.x(),
				this->attachedLine->getStartPoint()->getY() + normalVector.y()
				);
	QPointF aboveEndPoint(
				this->attachedLine->getEndPoint()->getX() + normalVector.x(),
				this->attachedLine->getEndPoint()->getY() + normalVector.y()
				);

	normalVector.normalize();
	normalVector *= this->textHeight;

	polygon << aboveStartPoint << aboveEndPoint;
	polygon << aboveEndPoint + QPointF(normalVector.x(),normalVector.y());
	polygon << aboveStartPoint + QPointF(normalVector.x(),normalVector.y());
	polygon << aboveStartPoint;

	QPainterPath path;
	path.setFillRule(Qt::FillRule::OddEvenFill);
	path.addPolygon(polygon);

	return path;
}

void LineLengthDimension::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(this->hidden)
		return;

	DrawableObject::paint(painter, option, widget);

	QVector2D lineVector = this->attachedLine->getLineVector().normalized();

	//get normal vector of line with defined length
	QVector2D normalVector(
				-lineVector.y(),
				lineVector.x()
						);
	normalVector *= this->distanceFromLine;

	//edge points
	QPointF aboveStartPoint(
				this->attachedLine->getStartPoint()->getX() + normalVector.x(),
				this->attachedLine->getStartPoint()->getY() + normalVector.y()
				);
	QPointF aboveEndPoint(
				this->attachedLine->getEndPoint()->getX() + normalVector.x(),
				this->attachedLine->getEndPoint()->getY() + normalVector.y()
				);

	//drawing the lines
	painter->drawLine(this->attachedLine->getStartPoint()->getLocation(),
						aboveStartPoint
					);
	painter->drawLine(aboveStartPoint, aboveEndPoint);
	painter->drawLine(aboveEndPoint,
						this->attachedLine->getEndPoint()->getLocation()
					  );

	//drawing text

	float textAngle = qRadiansToDegrees(
				qAtan(normalVector.x() / normalVector.y())
										);

	QPointF	centerPoint(
				(aboveStartPoint.x() + aboveEndPoint.x()) / 2,
				(aboveStartPoint.y() + aboveEndPoint.y()) / 2
				);

	painter->save();
	painter->translate(centerPoint);
	//rotate funciton rotates clockwise, we have angle counter clockwise
	painter->rotate(-textAngle);

	QRectF textRect(-this->textWidth / 2, -this->textHeight, this->textWidth, this->textHeight);
	painter->drawText(textRect, QString::number(this->lengthToSet));

	painter->resetTransform();
	painter->restore();
}

//---------     events     ----------

void LineLengthDimension::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	emit requestDouble(this);
}
