#include "LineLenghtDimension.h"

LineLenghtDimension::LineLenghtDimension(Line *line, double lenght)
{
	this->attachedLine = line;
	this->lengthToSet = lenght;
	this->distanceFromLine = 20;
}

void LineLenghtDimension::resolveTies()
{
	if(this->lengthToSet > 0)
		this->attachedLine->setLength(this->lengthToSet);
}

QRectF LineLenghtDimension::boundingRect() const
{

}

void LineLenghtDimension::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(this->hidden)
		return;

	resolveTies();

	double multiplier = pow(this->distanceFromLine, 2) / pow(this->attachedLine->getLineVector().x, 2);
	Vector2D normal_vector(
				sqrt(-pow(this->attachedLine->getLineVector().y, 2) * multiplier),
				sqrt(pow(this->attachedLine->getLineVector().x, 2) * multiplier)
				);

	QPoint aboveStartPoint(
				this->attachedLine->getStartPoint()->getX() + normal_vector.x,
				this->attachedLine->getStartPoint()->getY() + normal_vector.y
				);
	QPoint aboveEndPoint(
				this->attachedLine->getEndPoint()->getX() + normal_vector.x,
				this->attachedLine->getEndPoint()->getY() + normal_vector.y
				);

	painter->drawLine(this->attachedLine->getStartPoint()->getLocation(),
						aboveStartPoint
					);
	painter->drawLine(aboveStartPoint, aboveEndPoint);
	painter->drawLine(aboveEndPoint,
						this->attachedLine->getEndPoint()->getLocation()
					  );
}
