#include "LinesAngleConstraint.h"

LinesAngleConstraint::LinesAngleConstraint() : DrawableObject(Global::LinesAngleConstraint)
{

}

LinesAngleConstraint::LinesAngleConstraint(Line *lineOne, Line *lineTwo) : DrawableObject(Global::LinesAngleConstraint)
{
	this->lines[0] = lineOne;
	this->lines[1] = lineTwo;

    calculateEdgePoints();

    double deltaOneX = this->commonPoint->getX() - this->edgePoints[0]->getX();
    double deltaOneY = this->commonPoint->getY() - this->edgePoints[0]->getY();

    double deltaTwoX = this->commonPoint->getX() - this->edgePoints[1]->getX();
    double deltaTwoY = this->commonPoint->getY() - this->edgePoints[1]->getY();

    this->angle = (qAtan2(deltaOneY, deltaOneX) - qAtan2(deltaTwoY, deltaTwoX));

    setGeometryUpdates();

    //set she shorter line as distance from center
    if(lines[0]->getLength() > lines[1]->getLength())
	    setDistanceFromCenter(lines[1]->getLength()-1);
    else
	    setDistanceFromCenter(lines[0]->getLength()-1);
}

void LinesAngleConstraint::resolveTies()
{
    double deltaX = this->edgePoints[1]->getX() - this->commonPoint->getX();
    double deltaY = this->edgePoints[1]->getY() - this->commonPoint->getY();
    double lineOneAngle = qAtan2(deltaY, deltaX);

	Point *followedPoint = nullptr;
    if(edgePointsLocations[0] != edgePoints[0]->getLocation())
    {
		followedPoint = edgePoints[0];
    }
	else if (edgePointsLocations[1] != edgePoints[1]->getLocation())
    {
		followedPoint = edgePoints[1];
	}

	if(followedPoint != nullptr)
	{
		followedPoint->setLocation(
			this->commonPoint->getX() + this->lines[0]->getLength() * qCos(this->angle + lineOneAngle),
			this->commonPoint->getY() + this->lines[0]->getLength() * qSin(this->angle + lineOneAngle)
				);
	}

    edgePointsLocations[0] = edgePoints[0]->getLocation();
    edgePointsLocations[1] = edgePoints[1]->getLocation();
}

void LinesAngleConstraint::loadVariables(QString input)
{
    QStringList varNames = {
            "angle",
            "distanceFromCenter"
    };

    QVector<QVariant> variables = fetchVariables(input, varNames);

    this->angle = variables[0].toDouble();
    this->distanceFromCenter = variables[1].toDouble();
}

QString LinesAngleConstraint::toFileString()
{
    DrawableObject::toFileString();
    this->fileAddVar("angle", this->angle);
    this->fileAddVar("distanceFromCenter", this->distanceFromCenter);
    this->fileAddVar("lines0", this->lines[0]->getId());
    this->fileAddVar("lines1", this->lines[1]->getId());
    return this->fileFinish();
}

void LinesAngleConstraint::loadRelations(QVector<DrawableObject *> list)
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

void LinesAngleConstraint::setValue(double angle)
{
    this->angle = angle;
}

void LinesAngleConstraint::setDistanceFromCenter(double distance)
{
    this->distanceFromCenter = distance;
}

QRectF LinesAngleConstraint::boundingRect() const
{
    double diameter = this->distanceFromCenter + Settings::lineAngleArcMargins.top();

    return QRectF(
                this->commonPoint->getLocation() - QPointF(diameter, diameter),
                this->commonPoint->getLocation() + QPointF(diameter, diameter)
                );
}

QPainterPath LinesAngleConstraint::shape() const
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

void LinesAngleConstraint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(this->isHidden())return;

    DrawableObject::paint(painter);

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

void LinesAngleConstraint::recieveDouble(double value)
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

void LinesAngleConstraint::calculateEdgePoints()
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

    commonPoint->setLocked(true);

    edgePointsLocations[0] = edgePoints[0]->getLocation();
    edgePointsLocations[1] = edgePoints[1]->getLocation();
}

void LinesAngleConstraint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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

void LinesAngleConstraint::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit requestDouble(this, "Angle");
}

void LinesAngleConstraint::setGeometryUpdates()
{
    this->lines[0]->addGeometryUpdate(this);
    this->lines[1]->addGeometryUpdate(this);
}

void LinesAngleConstraint::unsetGeometryUpdates()
{
    this->lines[0]->removeGeometryUpdate(this);
    this->lines[1]->removeGeometryUpdate(this);
}
