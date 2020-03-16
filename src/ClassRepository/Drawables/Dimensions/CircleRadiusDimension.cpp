#include "CircleRadiusDimension.h"

CircleRadiusDimension::CircleRadiusDimension() : DrawableObject (Type_CircleRadiusDimension){}

CircleRadiusDimension::CircleRadiusDimension(Circle *circle) : DrawableObject (Type_CircleRadiusDimension)
{
	this->circle = circle;
	this->radius = circle->getRadius();
}

void CircleRadiusDimension::resolveTies()
{
	this->circle->setRadius(this->radius);
}

//----------     file handling     ----------

void CircleRadiusDimension::loadVariables(QString input)
{
	QStringList varNames = {
		"radius"
	};

	QVector<QVariant> variables = fetchVariables(input, varNames);

	this->radius = variables[0].toDouble();
}

QString CircleRadiusDimension::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("radius", this->radius);
	this->fileAddVar("circle", this->circle->getId());
	return DrawableObject::fileFinish();
}

void CircleRadiusDimension::loadRelations(QVector<DrawableObject *> *list)
{
	QStringList varNames = {
		"circle"
	};

	QVector<DrawableObject*> values = fetchRelations(list, varNames);

	this->circle = dynamic_cast<Circle*>(values[0]);
	setGeometryUpdates();
}

//----------     getters and setters     ----------

void CircleRadiusDimension::setRadius(double value)
{
	this->radius = value;
}

double CircleRadiusDimension::getRadius()
{
	return this->radius;
}

//----------     QGraphicsItem overrides     ----------

QRectF CircleRadiusDimension::boundingRect() const
{
	QPointF centerPoint = this->circle->getCenterPoint()->getLocation();
	QPointF rightPoint(centerPoint.x() + this->radius, centerPoint.y()-this->textHeight);

	return  QRectF(centerPoint, rightPoint);
}

QPainterPath CircleRadiusDimension::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());

	return path;
}

void CircleRadiusDimension::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(this->isHidden())
		return;

	DrawableObject::paint(painter, option, widget);

	QPointF centerPoint = this->circle->getCenterPoint()->getLocation();
	QPointF rightPoint(centerPoint.x() + this->radius, centerPoint.y());

	painter->drawLine(centerPoint, rightPoint);
	painter->drawText(QPointF(centerPoint.x() + radius / 2, centerPoint.y()), QString::number(this->radius));

	painter->setPen(Qt::red);
	painter->drawRect(boundingRect());
	painter->setPen(Qt::blue);
	painter->drawPath(shape());
}

//----------     events     ----------

void CircleRadiusDimension::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	emit requestDouble(this);
}

//----------     geometry     ---------

void CircleRadiusDimension::setGeometryUpdates()
{
	this->circle->addGeometryUpdate(this);
}

//----------     user input requests      ---------

void CircleRadiusDimension::recieveDouble(double value)
{
	this->radius = value;
}
