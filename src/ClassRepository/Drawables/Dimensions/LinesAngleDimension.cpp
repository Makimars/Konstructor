#include "LinesAngleDimension.h"

LinesAngleDimension::LinesAngleDimension() : DrawableObject (Type_LineAngleDimension){}

LinesAngleDimension::LinesAngleDimension(Line *lines[]) : DrawableObject (Type_LineAngleDimension)
{
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

void LinesAngleDimension::fromFileString(QString input)
{
	DrawableObject::fromFileString(input);

	QStringList varNames = {
		"angle",
		"distanceFromCenter"
	};

	QStringList variables = input.split(',');
	for(int i = 0; i < variables.length() - 1; i++)
	{
		QStringList parts = variables[i].split(":");
		QString varName = parts[0];
		QString varValue = parts[1];

		switch (varNames.indexOf(varName)) {
			case 0:
				this->angle = QVariant(varValue).toDouble();
				break;
			case 1:
				this->distanceFromCenter = QVariant(varValue).toDouble();
				break;
			default:
				break;
		}

	}
}

QString LinesAngleDimension::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("angle", this->angle);
	this->fileAddVar("distanceFromCenter", this->distanceFromCenter);
	this->fileAddVar("lines0", this->lines[0]->getId());
	this->fileAddVar("lines1", this->lines[1]->getId());
	return this->fileFinish();
}

void LinesAngleDimension::loadRelations(QVector<DrawableObject *> *list)
{
	QStringList varNames = {
		"lines0",
		"lines1"
	};

	QStringList variables = this->getFile().split(',');
	for(int i = 0; i < variables.length() - 1; i++)
	{
		QStringList parts = variables[i].split(":");
		QString varName = parts[0];
		QString varValue = parts[1];

		DrawableObject *obj;

		switch (varNames.indexOf(varName)) {
			case 0:
				obj = DrawableObject::getById(list, QVariant(varValue).toUInt());
				if(obj->getType() == Type_Line)
					this->lines[0] = dynamic_cast<Line*>(obj);
				break;
			case 1:
				obj = DrawableObject::getById(list, QVariant(varValue).toUInt());
				if(obj->getType() == Type_Line)
					this->lines[1] = dynamic_cast<Line*>(obj);
				break;
			default:
				break;
		}

	}
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
	if(this->isHidden())
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
	if(this->isDraging())
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
