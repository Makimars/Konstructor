#include "Circle.h"

Circle::Circle() : DrawableObject (Global::Circle){}

Circle::Circle(Point *center_point) : DrawableObject (Global::Circle)
{
	this->radius = 0;
	this->centerPoint = center_point;
	setGeometryUpdates();
}

void Circle::resolveTies()
{
	updateGeometry();
}

//----------	file handling    ----------

void Circle::loadVariables(QString input)
{
	QStringList varNames = {
		"radius"
    };

	QVector<QVariant> variables = fetchVariables(input, varNames);

	this->radius = variables[0].toDouble();
}

QString Circle::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("centerPoint", this->centerPoint->getId());
	this->fileAddVar("radius", this->radius);
	return this->fileFinish();
}

void Circle::loadRelations(QVector<DrawableObject*> list)
{
	QStringList varNames = {
		"centerPoint"
    };

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->centerPoint = dynamic_cast<Point*>(values[0]);
	setGeometryUpdates();
}

//----------	getters and setters    ----------

Circle *Circle::setRadius(double value)
{
	prepareGeometryChange();
	this->radius = value;
	updateGeometry();

	return this;
}

//----------	QGraphicsItem overrides    ----------

QRectF Circle::boundingRect() const
{
	QPointF top_left_point(
				this->centerPoint->getLocation() -
				QPointF(radius, radius)
				);

	return QRectF(top_left_point,
			  QSizeF(this->radius + radius,
					 this->radius + radius
					 )
			  );
}

QPainterPath Circle::shape() const
{
	QPainterPath path;
	path.addEllipse(boundingRect() + QMargins(1,1,1,1));

	QPainterPath smaller;
	smaller.addEllipse(boundingRect() - QMargins(2,2,2,2));

	return path.subtracted(smaller);
}

void Circle::paint(QPainter *painter,
				   const QStyleOptionGraphicsItem *option,
				   QWidget *widget)
{
	if(this->isHidden())return;

	DrawableObject::paint(painter);

	painter->drawEllipse(this->boundingRect());
}

//----------     geometry     ---------

void Circle::setGeometryUpdates()
{
	this->centerPoint->addGeometryUpdate(this);
}

void Circle::unsetGeometryUpdates()
{
	this->centerPoint->removeGeometryUpdate(this);
}

void Circle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	this->setRadius(centerPoint->distanceFrom(event->pos()));
}
