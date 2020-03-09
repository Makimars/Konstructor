#include "Point.h"

Point::Point() : DrawableObject(Type_Point){}

Point::Point(QPointF location) : DrawableObject (Type_Point)
{
    this->x = location.x();
    this->y = location.y();
}

Point::Point(double x, double y) : DrawableObject (Type_Point)
{
	this->x = x;
	this->y = y;
}

//----------	file handling    ----------

void Point::loadVariables(QString input)
{
	QStringList varNames = {
        "x",
        "y"
    };

	QVector<QVariant> variables = fetchVariables(input, varNames);

	this->x = variables[0].toDouble();
	this->y = variables[1].toDouble();
}

QString Point::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("x", this->x);
	this->fileAddVar("y", this->y);
    return DrawableObject::fileFinish();
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

Point *Point::setLocation(QPointF point)
{
    this->x = point.x();
	this->y = point.y();

	return this;
}

Point *Point::setLocation(double x, double y)
{
	this->x = x;
    this->y = y;

	return this;
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
	return QRectF(
				this->x + Settings::pointRenderSize/2,
				this->y + Settings::pointRenderSize/2,
				Settings::pointRenderSize,
				Settings::pointRenderSize
				);
}

QPainterPath Point::shape() const
{
	QPainterPath path;
	path.addEllipse(boundingRect());

	return path;
}

void Point::paint(QPainter *painter,
				  const QStyleOptionGraphicsItem *option,
				  QWidget *widget)
{    
	if(this->isHidden())
		return;

	DrawableObject::paint(painter, option, widget);

	painter->setBrush(this->brush->style());
	painter->setPen(this->pen->style());
	this->brush->setStyle(Qt::BrushStyle::SolidPattern);

	int coef = Settings::pointRenderSize;
	if(this->isHighlighted())
        coef *= 2;

    QRectF rect(this->x-coef, this->y-coef, coef  + coef, coef + coef);

	painter->drawEllipse(rect);
}
