#include "Point.h"

Point::Point() : DrawableObject(Type_Point){}

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
	setLocation(point.x(), point.y());

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
	Point *p = new Point();
	p->setLocation(getLocation());
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
				this->x - Settings::pointRenderSize,
				this->y - Settings::pointRenderSize,
				Settings::pointRenderSize * 2,
				Settings::pointRenderSize * 2
				) + Settings::pointMargin;
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
	this->brush->setStyle(Qt::BrushStyle::SolidPattern);

	QRectF rect;

	if(this->isHighlighted())
	{
		rect = boundingRect();
	}
	else
	{
		rect = QRectF(
					this->x-Settings::pointRenderSize,
					this->y-Settings::pointRenderSize,
					Settings::pointRenderSize * 2,
					Settings::pointRenderSize * 2
					);
	}

	painter->drawEllipse(rect);
}

//---------     events     ----------

void Point::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->isDraging())
	{
		this->setLocation(event->pos());
	}
}
