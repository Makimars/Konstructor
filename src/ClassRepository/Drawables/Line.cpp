#include "Line.h"
#include "QException"
Line::Line() : DrawableObject (Type_Line){}


Line::Line(Point *startPoint, Point *endPoint) : DrawableObject (Type_Line)
{
	this->startPoint = startPoint;
	this->endPoint = endPoint;
}

//----------	file handling    ----------

void Line::loadVariables(QString input)
{
	fetchVariables(input);
}

QString Line::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("startPoint", this->startPoint->getId());
	this->fileAddVar("endPoint", this->endPoint->getId());
    return DrawableObject::fileFinish();
}

void Line::loadRelations(QVector<DrawableObject*> *list)
{
    QStringList varNames = {
		"startPoint",
		"endPoint"
    };

	QVector<DrawableObject*> values = fetchRelations(list, varNames);

	this->startPoint = dynamic_cast<Point*>(values[0]);
	this->startPoint = dynamic_cast<Point*>(values[1]);

}

//----------	getters and setters    ----------

double Line::getLength()
{
	return this->startPoint->distanceFrom(endPoint->getLocation());
}

Line *Line::setLength(float lenght)
{
	QVector2D newVector = this->getLineVector().normalized() * lenght;

	this->endPoint
			->setLocation(this->startPoint->getX() + newVector.x(),
						this->startPoint->getY() + newVector.y()
						);

	return this;
}

QVector2D Line::getLineVector() const
{
	return QVector2D(
				this->endPoint->getX() - this->startPoint->getX(),
				this->endPoint->getY() - this->startPoint->getY()
				);
}

Line *Line::setLineVector(QVector2D vector)
{
	vector.normalize();
	vector *= this->getLength();
	QVector2D lineVector = this->getLineVector();

    this->endPoint->setX(
				this->startPoint->getY() + lineVector.x()
                );
    this->endPoint->setY(
				this->startPoint->getY() + lineVector.y()
                );

	return this;
}

Point *Line::getStartPoint()
{
	return this->startPoint;
}

Point *Line::getEndPoint()
{
	return this->endPoint;
}

Line *Line::clone()
{
	Line *l = new Line(this->startPoint, this->endPoint);
	l->setName(this->getName());

	return l;
}


//----------    Geometry    ----------

double Line::getAngle(QVector2D referenceVector)
{
	getLineVector();
	QVector2D lineVector = this->getLineVector();
	double scalarMult = (
						lineVector.x() * referenceVector.x()
						+ lineVector.y() * referenceVector.y()
						);

	return qAcos(
				scalarMult /
				this->getLength() * referenceVector.length()
				 );
}

Line *Line::setAngle(double angle,QVector2D referenceVector)
{
	double angleDifference = angle - this->getAngle(referenceVector);

	this->endPoint->setLocation(
				this->endPoint->getX() * (qCos(angleDifference) - qSin(angleDifference)),
				this->endPoint->getY() * (qSin(angleDifference) - qCos(angleDifference))
				);

	return this;
}

//----------     Distance    ----------

double Line::distanceFrom(QPointF location)
{
	return abs(signedDistanceFrom(location));
}

double Line::signedDistanceFrom(QPointF location)
{
	double denominator = (
				((this->endPoint->getY() - this->startPoint->getY()) * location.x()) -
				((this->endPoint->getX() - this->startPoint->getX()) * location.y()) +
				(this->endPoint->getX() * this->startPoint->getY()) -
				(this->endPoint->getY() * this->startPoint->getX())
				);
	double numerator = sqrt(
				pow(this->endPoint->getY() - this->startPoint->getY(), 2) +
				pow(this->endPoint->getX() - this->startPoint->getX(), 2)
				);

	return denominator / numerator;
}

//----------	QGraphicsItem overrides    ----------

QRectF Line::boundingRect() const
{
	QVector2D lineVector = this->getLineVector().normalized();
	QVector2D normalVector(
				-lineVector.y(),
				lineVector.x()
						);
	QPointF startPointOne(this->startPoint->getLocation()+normalVector.toPointF());
	QPointF startPointTwo(this->startPoint->getLocation()-normalVector.toPointF());

	QPointF endPointOne(this->endPoint->getLocation()+normalVector.toPointF());
	QPointF endPointTwo(this->endPoint->getLocation()-normalVector.toPointF());

	QList<double> xList;
	xList << startPointOne.x() << startPointTwo.x() << endPointOne.x() << endPointTwo.x();

	QList<double> yList;
	yList << startPointOne.y() << startPointTwo.y() << endPointOne.y() << endPointTwo.y();

	std::sort(xList.begin(), xList.end(), std::greater<double>());
	std::sort(yList.begin(), yList.end(), std::greater<double>());
	double topX = xList[0];
	double topY = yList[0];

	std::sort(xList.begin(), xList.end(), std::less<double>());
	std::sort(yList.begin(), yList.end(), std::less<double>());
	double bottomX = xList[0];
	double bottomY = yList[0];

	return QRectF(QPointF(topX,topY), QPointF(bottomX, bottomY));
}

QPainterPath Line::shape() const
{
	QVector2D lineVector = this->getLineVector().normalized();
	QVector2D normalVector(
				-lineVector.y(),
				lineVector.x()
						);
	QPointF startPointOne(this->startPoint->getLocation()+normalVector.toPointF());
	QPointF startPointTwo(this->startPoint->getLocation()-normalVector.toPointF());

	QPointF endPointOne(this->endPoint->getLocation()+normalVector.toPointF());
	QPointF endPointTwo(this->endPoint->getLocation()-normalVector.toPointF());

	QPolygonF polygon;
	polygon << startPointOne << startPointTwo << endPointTwo << endPointOne << startPointOne;

	QPainterPath path;
	path.addPolygon(polygon);

	return path;
}

void Line::paint(QPainter *painter,
				 const QStyleOptionGraphicsItem *option,
				 QWidget *widget)
{
	if(this->isHidden())
		return;

	DrawableObject::paint(painter, option, widget);

	painter->drawLine(this->startPoint->getLocation(),
						this->endPoint->getLocation()
					);
}


