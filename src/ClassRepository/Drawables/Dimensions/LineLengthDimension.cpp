#include "LineLengthDimension.h"

LineLengthDimension::LineLengthDimension() : DrawableObject (Type_LineLengthDimension)
{
	this->distanceFromLine = 0;
	this->distanceFromLine = 20;
}

LineLengthDimension::LineLengthDimension(Line *line) : DrawableObject (Type_LineLengthDimension)
{
	LineLengthDimension();
	this->attachedLine = line;
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

void LineLengthDimension::setLineLength(double length)
{
	this->lengthToSet = length;
}

//---------    file handeling     ---------

void LineLengthDimension::fromFileString(QString input)
{
	DrawableObject::fromFileString(input);

	QStringList varNames = {
		"lengthToSet",
		"distanceFromLine"
	};

	QStringList variables = input.split(',');
	for(int i = 0; i < variables.length() - 1; i++)
	{
		QStringList parts = variables[i].split(":");
		QString varName = parts[0];
		QString varValue = parts[1];

		switch (varNames.indexOf(varName)) {
			case 0:
				this->lengthToSet = QVariant(varValue).toDouble();
				break;
			case 1:
				this->distanceFromLine = QVariant(varValue).toDouble();
				break;
			default:
				break;
		}

	}
}

QString LineLengthDimension::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("lengthToSet", this->lengthToSet);
	this->fileAddVar("distanceFromLine", this->distanceFromLine);
	this->fileAddVar("attachedLine", this->attachedLine->getId());
	return this->fileFinish();
}

void LineLengthDimension::loadRelations(QVector<DrawableObject *> *list)
{
	QStringList varNames = {
		"attachedLine"
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
					this->attachedLine = dynamic_cast<Line*>(obj);

				break;
			default:
				break;
		}

	}
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
	if(this->isHidden())
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

	double textAngle = qRadiansToDegrees(
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
	if(lineVector.x() < 0 & this->distanceFromLine < 0	||
		lineVector.x() > 0 & this->distanceFromLine > 0	)
		painter->translate(QPointF(0,textHeight));

	QRectF textRect(-this->textWidth / 2, -this->textHeight, this->textWidth, this->textHeight);
	painter->drawText(textRect, QString::number(this->lengthToSet));

	painter->resetTransform();
	painter->restore();
}

//---------     events     ----------

void LineLengthDimension::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->isDraging())
	{
		QPointF mousePos = event->pos();
		this->distanceFromLine = -this->attachedLine->signedDistanceFrom(mousePos);
	}
}

void LineLengthDimension::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	emit requestDouble(this);
}


//--------     user input requests     ---------

void LineLengthDimension::recieveDouble(double value)
{
	this->lengthToSet = value;
	resolveTies();
}
