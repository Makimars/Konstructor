#include "Point.h"

Point::Point() : DrawableObject(Global::Point)
{
	setZValue(POINT_Z_LEVEL);
}

Point::Point(double x, double y) : Point()
{
	setLocation(x,y);
}

//----------	file handling    ----------

void Point::loadData(nlohmann::json jsonInput)
{
	DrawableObject::loadData(jsonInput);
	this->x = jsonInput["x"];
	this->y = jsonInput["y"];
}

nlohmann::json Point::toJson()
{
	DrawableObject::toJson();
	json["x"] = x;
	json["y"] = y;

	return json;
}

//----------	getters and setters    ----------

void Point::setLocation(double x, double y)
{
	if(isLocked()) return;

	updateGeometry();
	this->x = x;
	this->y = y;
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
	if(this->isHidden()) return;

	DrawableObject::paint(painter);

	if(isLocked() | isConstrained())
	{
		painter->setBrush(QBrush(Qt::red));

		QPen pen = painter->pen();
		pen.setColor(Qt::red);
		painter->setPen(pen);
	}

	QRectF rect = QRectF(
				this->x - Settings::pointRenderSize,
				this->y - Settings::pointRenderSize,
				Settings::pointRenderSize * 2,
				Settings::pointRenderSize * 2
				);

	if(this->isHighlighted())
		rect += Settings::pointMargin;

	painter->drawEllipse(rect);
}

//---------     events     ----------

void Point::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->isDraging() & !isLocked() & !isConstrained())
	{
		this->setLocation(event->pos());
	}
}
