#include "Line.h"

Line::Line()
{
	this->type = TYPE_LINE;
}


Line::Line(Point *startPoint, Point *endPoint) : DrawableObject()
{
	this->type = TYPE_LINE;
	this->startPoint = startPoint;
	this->endPoint = endPoint;
	this->lineVector = QVector2D(
				this->endPoint->getX() - this->startPoint->getX(),
				this->endPoint->getY() - this->startPoint->getY()
				);
}

//----------	file handling    ----------

void Line::fromFileString(QString input)
{
	DrawableObject::fromFileString(input);
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

    QStringList variables = this->file.split(',');
    for(int i = 0; i < variables.length() - 1; i++)
    {
        QStringList parts = variables[i].split(":");
        QString varName = parts[0];
        QString varValue = parts[1];

		DrawableObject *object;

        switch (varNames.indexOf(varName)) {
            case 0:
				object = DrawableObject::getById(list, QVariant(varValue).toUInt());
				if(object->getType() == TYPE_POINT)
					this->startPoint = dynamic_cast<Point*>(object);

                break;
            case 1:
				object = DrawableObject::getById(list, QVariant(varValue).toUInt());
				if(object->getType() == TYPE_POINT)
					this->endPoint = dynamic_cast<Point*>(object);

                break;
            default:
                break;
        }

    }

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

QVector2D Line::getLineVector()
{
	this->lineVector =QVector2D(
				this->endPoint->getX() - this->startPoint->getX(),
				this->endPoint->getY() - this->startPoint->getY()
				);
	return this->lineVector;
}

Line *Line::setLineVector(QVector2D vector)
{
	vector.normalize();
	vector *= this->getLength();

    this->endPoint->setX(
				this->startPoint->getY() + this->lineVector.x()
                );
    this->endPoint->setY(
				this->startPoint->getY() + this->lineVector.y()
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
	double scalarMult = (
						this->lineVector.x() * referenceVector.x()
						+ this->lineVector.y() * referenceVector.y()
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
	QPointF first = this->startPoint->getLocation();
	QPointF second = this->endPoint->getLocation();

    return QRectF(first, second);
}

QPainterPath Line::shape() const
{
	QPainterPath path;
	path.moveTo(this->startPoint->getLocation());
	path.lineTo(this->endPoint->getLocation());

	return path;
}

void Line::paint(QPainter *painter,
				 const QStyleOptionGraphicsItem *option,
				 QWidget *widget)
{
	if(this->hidden)
		return;

	DrawableObject::paint(painter, option, widget);

	painter->drawLine(this->startPoint->getLocation(),
						this->endPoint->getLocation()
					);
}


