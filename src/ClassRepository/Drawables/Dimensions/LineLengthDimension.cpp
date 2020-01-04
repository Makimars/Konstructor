#include "LineLengthDimension.h"

LineLengthDimension::LineLengthDimension(Line *line, double length)
{
	this->attachedLine = line;
	this->lengthToSet = length;
	this->distanceFromLine = 20;
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
	//get normal vector of line
	QVector2D normalVector = this->attachedLine->getLineVector().normalized();
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

	int textWidth = 60;
	int textHeight = 20;
	QRectF textRect(-textWidth / 2, -textHeight, textWidth, textHeight);
	painter->drawText(textRect, QString::number(this->lengthToSet));

	painter->resetTransform();
	painter->restore();
}
