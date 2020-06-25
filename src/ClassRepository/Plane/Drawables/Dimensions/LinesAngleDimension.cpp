#include "LinesAngleDimension.h"

LinesAngleDimension::LinesAngleDimension() : DrawableObject (Global::Type_LineAngleDimension){}

LinesAngleDimension::LinesAngleDimension(Line *lines[]) : DrawableObject (Global::Type_LineAngleDimension)
{
	this->lines[0] = lines[0];
	this->lines[1] = lines[1];

	calculateEdgePoints();

	double deltaOneX = this->commonPoint->getX() - this->edgePoints[0]->getX();
	double deltaOneY = this->commonPoint->getY() - this->edgePoints[0]->getY();

	double deltaTwoX = this->commonPoint->getX() - this->edgePoints[1]->getX();
	double deltaTwoY = this->commonPoint->getY() - this->edgePoints[1]->getY();

	this->angle = (qAtan2(deltaOneY, deltaOneX) - qAtan2(deltaTwoY, deltaTwoX));

	setGeometryUpdates();
}

void LinesAngleDimension::calculateEdgePoints()
{
	if((this->lines[0]->getStartPoint() == this->lines[1]->getStartPoint())	|
		(this->lines[0]->getStartPoint() == this->lines[1]->getEndPoint())	)
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
}

void LinesAngleDimension::resolveTies()
{
	double deltaX = this->edgePoints[1]->getX() - this->commonPoint->getX();
	double deltaY = this->edgePoints[1]->getY() - this->commonPoint->getY();
	double lineOneAngle = qAtan2(deltaY, deltaX);

	this->edgePoints[0]->setLocation(
				this->commonPoint->getX() + this->lines[0]->getLength() * qCos(this->angle + lineOneAngle),
				this->commonPoint->getY() + this->lines[0]->getLength() * qSin(this->angle + lineOneAngle)
			);
}

//----------     file handling     ---------

void LinesAngleDimension::loadVariables(QString input)
{
	QStringList varNames = {
		"angle",
		"distanceFromCenter"
	};

	QVector<QVariant> variables = fetchVariables(input, varNames);

	this->angle = variables[0].toDouble();
	this->distanceFromCenter = variables[1].toDouble();
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

void LinesAngleDimension::loadRelations(QVector<DrawableObject*> list)
{
	QStringList varNames = {
		"lines0",
		"lines1"
	};

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->lines[0] = dynamic_cast<Line*>(values[0]);
	this->lines[1] = dynamic_cast<Line*>(values[1]);

	calculateEdgePoints();
	setGeometryUpdates();
}

//----------	QGraphicsItem overrides    ----------

QRectF LinesAngleDimension::boundingRect() const
{
	double diameter = this->distanceFromCenter + Settings::lineAngleArcMargins.top();

	return QRectF(
				this->commonPoint->getLocation() - QPointF(diameter, diameter),
				this->commonPoint->getLocation() + QPointF(diameter, diameter)
				);
}

QPainterPath LinesAngleDimension::shape() const
{
	QVector2D lineVector = lines[0]->getLineVector();

	if(this->lines[0]->getEndPoint() == this->commonPoint)
		lineVector *= -1;

	QPointF virtualPoint = this->edgePoints[1]->getLocation() + lineVector.toPointF();
	QPolygonF shape;
	shape << this->commonPoint->getLocation()
		  << this->edgePoints[0]->getLocation()
		  << virtualPoint
		  << this->edgePoints[1]->getLocation()
		  << this->commonPoint->getLocation();

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
		return;

	DrawableObject::paint(painter, option, widget);

	QPainterPath outerCircle;
	outerCircle.addEllipse(this->commonPoint->getLocation(),this->distanceFromCenter, this->distanceFromCenter);

	QPainterPath innerCircle;
	innerCircle.addEllipse(this->commonPoint->getLocation(),this->distanceFromCenter - 0.5, this->distanceFromCenter - 0.5);

	painter->drawPath(outerCircle.subtracted(innerCircle).intersected(shape()));

	QString angleText;
	if(Settings::angleUnits == AngleUnits::degrees)
	{
		angleText = QString::number(qRadiansToDegrees(this->angle));
	}
	else
	{
		angleText = QString::number(this->angle);
	}
	painter->drawText(this->edgePoints[0]->getLocation(), angleText);
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
	if(Settings::angleUnits == AngleUnits::degrees)
	{
		this->angle = qDegreesToRadians(value);
	}
	else
	{
		this->angle = value;
	}
}

void LinesAngleDimension::setGeometryUpdates()
{
	this->lines[0]->addGeometryUpdate(this);
	this->lines[1]->addGeometryUpdate(this);
}

void LinesAngleDimension::unsetGeometryUpdates()
{
	this->lines[0]->removeGeometryUpdate(this);
	this->lines[1]->removeGeometryUpdate(this);
}
