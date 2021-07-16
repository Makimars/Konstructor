#include "CircleRadiusConstraint.h"

CircleRadiusConstraint::CircleRadiusConstraint() : DrawableObject (Global::CircleRadiusConstraint){}

CircleRadiusConstraint::CircleRadiusConstraint(Circle *circle) : DrawableObject (Global::CircleRadiusConstraint)
{
	this->circle = circle;
	this->radius = circle->getRadius();
	circle->addConstraint();

	setGeometryUpdates();
}

CircleRadiusConstraint::~CircleRadiusConstraint()
{
	circle->removeConstraint();
}

void CircleRadiusConstraint::resolveTies()
{
	this->circle->setRadius(this->radius);
}

//----------     file handling     ----------

void CircleRadiusConstraint::loadData(nlohmann::json jsonInput)
{
	DrawableObject::loadData(jsonInput);
	this->radius = jsonInput["radius"];
}

void CircleRadiusConstraint::loadRelations(QVector<DrawableObject*> list)
{
	QStringList varNames = {
		"circle"
	};

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->circle = dynamic_cast<Circle*>(values[0]);
	setGeometryUpdates();
	circle->addConstraint();
}

nlohmann::json CircleRadiusConstraint::toJson()
{
	DrawableObject::toJson();
	json["circle"] = this->circle->getId();
	json["radius"] = this->radius;

	return json;
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
