#include "CircleRadiusConstraint.h"

CircleRadiusConstraint::CircleRadiusConstraint() : DrawableObject (Global::CircleRadiusDimension){}

CircleRadiusConstraint::CircleRadiusConstraint(Circle *circle) : DrawableObject (Global::CircleRadiusDimension)
{
	this->circle = circle;
	this->radius = circle->getRadius();

	setGeometryUpdates();
}

void CircleRadiusConstraint::resolveTies()
{
	this->circle->setRadius(this->radius);
}

//----------     file handling     ----------

void CircleRadiusConstraint::loadVariables(QString input)
{
	QStringList varNames = {
		"radius"
	};

	QVector<QVariant> variables = fetchVariables(input, varNames);

	this->radius = variables[0].toDouble();
}

QString CircleRadiusConstraint::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("radius", this->radius);
	this->fileAddVar("circle", this->circle->getId());
	return DrawableObject::fileFinish();
}

void CircleRadiusConstraint::loadRelations(QVector<DrawableObject*> list)
{
	QStringList varNames = {
		"circle"
	};

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->circle = dynamic_cast<Circle*>(values[0]);
	setGeometryUpdates();
}

//----------     QGraphicsItem overrides     ----------

QRectF CircleRadiusConstraint::boundingRect() const
{
	QPointF centerPoint = this->circle->getCenterPoint()->getLocation();
	QPointF rightPoint(centerPoint.x() + this->radius, centerPoint.y()-this->textHeight);

	return  QRectF(centerPoint, rightPoint);
}

QPainterPath CircleRadiusConstraint::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());

	return path;
}

void CircleRadiusConstraint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(this->isHidden())return;

	DrawableObject::paint(painter);

	QPointF centerPoint = this->circle->getCenterPoint()->getLocation();
	QPointF rightPoint(centerPoint.x() + this->radius, centerPoint.y());

	painter->drawLine(centerPoint, rightPoint);
	painter->drawText(QPointF(centerPoint.x() + radius / 2, centerPoint.y()), QString::number(this->radius));
}

//----------     events     ----------

void CircleRadiusConstraint::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	emit requestDouble(this, "Radius");
}

//----------     geometry     ---------

void CircleRadiusConstraint::setGeometryUpdates()
{
	this->circle->addGeometryUpdate(this);
}

void CircleRadiusConstraint::unsetGeometryUpdates()
{
	this->circle->removeGeometryUpdate(this);
}

//----------     user input requests      ---------

void CircleRadiusConstraint::recieveDouble(double value)
{
	this->radius = value;
}
