#include "CirclesRadiusDifferenceDimension.h"

CirclesRadiusDifferenceDimension::CirclesRadiusDifferenceDimension() : DrawableObject(Global::CirclesRadiusDifferenceDimension){}

CirclesRadiusDifferenceDimension::CirclesRadiusDifferenceDimension(Circle *circles[]) : DrawableObject(Global::CirclesRadiusDifferenceDimension)
{
	this->circles[0] = circles[0];
	this->circles[1] = circles[1];
	this->radiusDifference = circles[1]->getRadius() - circles[0]->getRadius();

	setGeometryUpdates();
}

void CirclesRadiusDifferenceDimension::resolveTies()
{
	this->circles[0]->setRadius(this->circles[1]->getRadius() + this->radiusDifference);
}

//----------     file handling     ---------

void CirclesRadiusDifferenceDimension::loadVariables(QString input)
{

	QStringList varNames = {
		"radiusDifference"
	};

	QVector<QVariant> variables = fetchVariables(input, varNames);

	this->radiusDifference = variables[0].toDouble();
}

QString CirclesRadiusDifferenceDimension::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("radiusDifference", this->radiusDifference);
	this->fileAddVar("circles0", this->circles[0]->getId());
	this->fileAddVar("circles1", this->circles[1]->getId());
	return this->fileFinish();
}

void CirclesRadiusDifferenceDimension::loadRelations(QVector<DrawableObject *> list)
{
	QStringList varNames = {
		"lines0",
		"lines1"
	};

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->circles[0] = dynamic_cast<Circle*>(values[0]);
	this->circles[1] = dynamic_cast<Circle*>(values[1]);
	this->radiusDifference = this->circles[1]->getRadius() - this->circles[0]->getRadius();

	setGeometryUpdates();
}

//----------	QGraphicsItem overrides    ----------

QRectF CirclesRadiusDifferenceDimension::boundingRect() const
{
	QPointF point = this->circles[1]->getCenterPoint()->getLocation() + QPointF(0,this->circles[0]->getRadius());

	return QRectF(point, point + QPointF(textWidth, textHeight));
}

QPainterPath CirclesRadiusDifferenceDimension::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());

	return path;
}

void CirclesRadiusDifferenceDimension::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(isHidden()) return;

	DrawableObject::paint(painter);

	QPointF firstPoint = this->circles[0]->getCenterPoint()->getLocation() + QPointF(0,this->circles[0]->getRadius()/2);
	QPointF secondPoint = firstPoint + QPointF(0, this->radiusDifference/2);

	painter->drawLine(firstPoint, secondPoint);
	painter->drawText(boundingRect(), QString::number(this->radiusDifference));

	painter->drawRect(boundingRect());
	painter->drawPath(shape());
}

void CirclesRadiusDifferenceDimension::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	emit requestDouble(this, "Radius diference");
}

void CirclesRadiusDifferenceDimension::setGeometryUpdates()
{
	this->circles[0]->addGeometryUpdate(this);
	this->circles[1]->addGeometryUpdate(this);
}

void CirclesRadiusDifferenceDimension::unsetGeometryUpdates()
{
	this->circles[0]->removeGeometryUpdate(this);
	this->circles[1]->removeGeometryUpdate(this);
}

void CirclesRadiusDifferenceDimension::recieveDouble(double value)
{
	this->radiusDifference = value;
}
