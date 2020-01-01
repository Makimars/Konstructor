#include "Point.h"

Point::Point() : DrawableObject()
{
	this->type = TYPE_POINT;
}

Point::Point(QPointF location)
{
	this->type = TYPE_POINT;
    this->x = location.x();
    this->y = location.y();
}

Point::Point(double x, double y) : DrawableObject()
{
	this->type = TYPE_POINT;
	this->x = x;
	this->y = y;
}

Point::~Point()
{

}

void Point::resolveTies()
{

}

//----------	file handling    ----------

void Point::fromFileString(QString input)
{
	DrawableObject::fromFileString(input);

    QStringList var_names = {
        "x",
        "y"
    };

    QStringList variables = input.split(',');
    for(int i = 0; i < variables.length() - 1; i++)
	{
		QStringList parts = variables[i].split(":");
		QString var_name = parts[0];
		QString var_value = parts[1];

		switch (var_names.indexOf(var_name)) {
			case 0:
				this->x = QVariant(var_value).toDouble();
				break;
			case 1:
				this->y = QVariant(var_value).toDouble();
				break;
			default:
				break;
		}
	}
}

QString Point::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("x", this->x);
	this->fileAddVar("y", this->y);
    return DrawableObject::fileFinish();
}

void Point::loadRelations(QVector<DrawableObject *> *list)
{

}

//----------	getters and setters    ----------

double Point::getX()
{
	return this->x;
}

double Point::getY()
{
	return this->y;
}

QPointF Point::getLocation()
{
	return QPointF(this->x,this->y);
}

void Point::setLocation(QPointF point)
{
    this->x = point.x();
	this->y = point.y();
}

void Point::setLocation(double x, double y)
{
	this->x = x;
    this->y = y;
}

Point *Point::clone()
{
	Point *p = new Point(this->x,this->y);
	p->setName(this->name)
		->setBrush(this->brush)
		->setPen(this->pen);

	return p;
}

//----------	aritmetic functions    ----------

double Point::distanceFrom(QPointF point)
{
    return qSqrt(pow(point.x() - this->x, 2) + pow(point.y() - this->y, 2));
}

//----------    QGraphicsItem overrides    ----------

QRectF Point::boundingRect() const
{
    return QRectF(this->x, this->y, 1, 1);
}

void Point::paint(QPainter *painter,
				  const QStyleOptionGraphicsItem *option,
				  QWidget *widget)
{    
	if(this->hidden)
		return;

	DrawableObject::paint(painter, option, widget);

	painter->setBrush(this->brush->style());
	painter->setPen(this->pen->style());

	int coef = Settings::pointRenderSize;
    if(this->highlight)
        coef *= 2;

    QRectF rect(this->x-coef, this->y-coef, coef  + coef, coef + coef);

	painter->drawEllipse(rect);
}
