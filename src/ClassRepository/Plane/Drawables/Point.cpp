#include "Point.h"

Point::Point() : DrawableObject(Global::Point){}

Point::Point(double x, double y) : DrawableObject(Global::Point)
{
	setLocation(x,y);
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

void Point::setLocation(double x, double y)
{
	if(isLocked()) return;

	updateGeometry();
	this->x = x;
	this->y = y;
}

Point *Point::clone()
{
	Point *p = new Point();
	p->setLocation(getLocation());
	p->setName(this->name);
	p->setBrush(this->brush);
	p->setPen(this->pen);

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
	if(this->isDraging() & !isLocked())
	{
		this->setLocation(event->pos());
	}
}
