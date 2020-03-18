#include "LineLengthDimension.h"

LineLengthDimension::LineLengthDimension() : DrawableObject (Type_LineLengthDimension){}

LineLengthDimension::LineLengthDimension(Line *line) : DrawableObject (Type_LineLengthDimension)
{
	this->attachedLine = line;
	setGeometryUpdates();
}

LineLengthDimension::~LineLengthDimension()
{
	this->attachedLine->removeGeometryUpdate(this);
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

void LineLengthDimension::loadVariables(QString input)
{
	QStringList varNames = {
		"lengthToSet",
		"distanceFromLine"
	};

	QVector<QVariant> variables = fetchVariables(input, varNames);

	this->lengthToSet = variables[0].toDouble();
	this->distanceFromLine = variables[1].toDouble();
}

QString LineLengthDimension::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("lengthToSet", this->lengthToSet);
	this->fileAddVar("distanceFromLine", this->distanceFromLine);
	this->fileAddVar("attachedLine", this->attachedLine->getId());
	return this->fileFinish();
}

void LineLengthDimension::loadRelations(QVector<DrawableObject*> list)
{
	QStringList varNames = {
		"attachedLine"
	};

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->attachedLine = dynamic_cast<Line*>(values[0]);
	setGeometryUpdates();
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

	//calculate rectangle edges
	double topX,topY;

	if(aboveStartPoint.y() > aboveEndPoint.y())
		topY = aboveStartPoint.y() + textHeight;
	else
		topY = aboveEndPoint.y() + textHeight;

	if(aboveStartPoint.x() > aboveEndPoint.x())
		topX = aboveStartPoint.x() + textWidth;
	else
		topX = aboveEndPoint.x() + textWidth;

	double bottomX,bottomY;

	if(aboveStartPoint.y() < aboveEndPoint.y())
		bottomY = aboveStartPoint.y() - textHeight;
	else
		bottomY = aboveEndPoint.y() - textHeight;

	if(aboveStartPoint.x() < aboveEndPoint.x())
		bottomX = aboveStartPoint.x() - textWidth;
	else
		bottomX = aboveEndPoint.x() - textWidth;

	return QRectF(QPointF(topX, topY),QPointF(bottomX, bottomY));
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

void LineLengthDimension::setGeometryUpdates()
{
	this->attachedLine->addGeometryUpdate(this);
}
