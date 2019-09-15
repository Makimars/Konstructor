#include "Line.h"

Line::Line()
{
	this->type = "Line";
}


Line::Line(Point *start_point, Point *end_point) : DrawableObject()
{
	this->type = "Line";
	this->start_point = start_point;
	this->end_point = end_point;
	this->line_vector = Vector2D(
				this->end_point->getX() - this->start_point->getX(),
				this->end_point->getY() - this->start_point->getY()
				);
}

Line::~Line()
{

}

void Line::resolveTies()
{

}

//----------	file handling    ----------

void Line::fromFileString(QString input)
{
	DrawableObject::fromFileString(input);
}

QString Line::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("start_point", this->start_point->getId());
	this->fileAddVar("end_point", this->end_point->getId());
    return DrawableObject::fileFinish();
}

void Line::loadRelations(QVector<DrawableObject> * list)
{

}

//----------	getters and setters    ----------

void Line::setLineVector(Vector2D vector)
{
	this->line_vector = vector;
}

Vector2D * Line::getLineVector()
{
	return &this->line_vector;
}

Point * Line::getStartPoint()
{
	return this->start_point;
}

Point * Line::getEndPoint()
{
	return this->end_point;
}

//----------	QGraphicsItem overrides    ----------

QRectF Line::boundingRect() const
{
	QPointF first = this->start_point->getLocation();
	QPointF second = this->end_point->getLocation();

	return QRectF(first, second);
}

void Line::paint(QPainter * painter,
				 const QStyleOptionGraphicsItem * option,
				 QWidget * widget)
{
	painter->drawLine(this->start_point->getLocation(),
					  this->end_point->getLocation()
					  );
}


