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

}

void LineLengthDimension::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(this->hidden)
		return;

	resolveTies();

	double multiplier = pow(this->distanceFromLine, 2) / pow(this->attachedLine->getLineVector().x, 2);

	Vector2D lineVector = this->attachedLine->getLineVector();

	//X direction multiplier
	int mult = 1;
	if((lineVector.x > 0 & lineVector.y < 0)	|
		(lineVector.x < 0 & lineVector.y > 0)	)
	{
		mult = -1;
	}

	//get normal vector of line
	Vector2D normalVector(
				mult * sqrt(pow(lineVector.y, 2) * multiplier),
				-sqrt(pow(lineVector.x, 2) * multiplier)
				);

	//edge points
	QPointF aboveStartPoint(
				this->attachedLine->getStartPoint()->getX() + normalVector.x,
				this->attachedLine->getStartPoint()->getY() + normalVector.y
				);
	QPointF aboveEndPoint(
				this->attachedLine->getEndPoint()->getX() + normalVector.x,
				this->attachedLine->getEndPoint()->getY() + normalVector.y
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

	double textAngle = qRadiansToDegrees(
				qAtan(normalVector.x / normalVector.y)
										);

	painter->rotate(-textAngle);

	double textHeight = 20;
	double textWidth = 30;
	QPointF	centerPoint(
				-mult * (aboveStartPoint.x() + ((aboveEndPoint.x() - aboveStartPoint.x()) / 2) - textWidth*2),
				mult * (aboveStartPoint.y() + ((aboveEndPoint.y() - aboveStartPoint.y()) / 2) - textHeight*2)
				);

	painter->drawText(centerPoint, QString::number(this->lengthToSet));

	painter->resetTransform();

	//test the location of center point
	this->pen->setWidth(10);
	this->brush->setStyle(Qt::BrushStyle::SolidPattern);
	painter->setPen(*this->pen);
	painter->setBrush(*this->brush);
	int coef = 40;
	QRectF rect(centerPoint.x() - coef, centerPoint.y() - coef, coef + coef, coef + coef);
	painter->drawEllipse(rect);

}
