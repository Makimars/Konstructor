#include "Circle.h"

Circle::Circle()
{
	this->type = "Circle";
}

Circle::Circle(Point * center_point, double radius)
{
	this->type = "Circle";
	this->center_point = center_point;
	this->radius = radius;
}

Circle::~Circle(){

}

void Circle::resolveTies()
{

}

//----------	file handling    ----------

void Circle::fromFileString(QString input)
{
	DrawableObject::fromFileString(input);

	QStringList var_names;
	var_names[0] = "radius";

	QStringList variables = input.split(',');
	for(unsigned long i = 0; i < variables.length() - 1; i++)
	{
		QStringList parts = variables[i].split(":");
		QString var_name = parts[0];
		QString var_value = parts[1];

		switch (var_names.indexOf(var_name)) {
			case 0:
				this->radius = QVariant(var_value).toDouble();
				break;
			default:
				break;
		}

	}
}

void Circle::loadRelations(QVector<DrawableObject> * list)
{

}

QString Circle::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("center_point", this->center_point->getId());
	this->fileAddVar("radius", this->radius);
	return this->fileFinish();
}

//----------	getters and setters    ----------

Point * Circle::getCenterPoint()
{
	return this->center_point;
}

double * Circle::getRadius()
{
	return &this->radius;
}

void Circle::setRadius(double value)
{
	this->radius = value;
}

//----------	QGraphicsItem overrides    ----------

QRectF Circle::boundingRect() const
{
	QPointF top_left_point(
				this->center_point->getLocation() -
				QPointF(radius, radius)
				);

	return QRectF(top_left_point,
			  QSizeF(this->radius + radius,
					 this->radius + radius
					 )
			  );
}

void Circle::paint(QPainter * painter,
				   const QStyleOptionGraphicsItem * option,
				   QWidget * widget)
{
	painter->drawEllipse(this->boundingRect());
}
