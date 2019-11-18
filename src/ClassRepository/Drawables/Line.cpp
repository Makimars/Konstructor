#include "Line.h"

Line::Line()
{
	this->type = TYPE_LINE;
}


Line::Line(Point *start_point, Point *end_point) : DrawableObject()
{
	this->type = TYPE_LINE;
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

void Line::loadRelations(QVector<DrawableObject*> *list)
{
    QStringList var_names = {
        "start_point",
        "end_point"
    };

    QStringList variables = this->file.split(',');
    for(int i = 0; i < variables.length() - 1; i++)
    {
        QStringList parts = variables[i].split(":");
        QString var_name = parts[0];
        QString var_value = parts[1];

		DrawableObject *obj;

        switch (var_names.indexOf(var_name)) {
            case 0:
                obj = DrawableObject::getById(list, QVariant(var_value).toUInt());
				if(obj->getType() == TYPE_POINT)
                    this->start_point = (Point*)obj;

                break;
            case 1:
                obj = DrawableObject::getById(list, QVariant(var_value).toUInt());
				if(obj->getType() == TYPE_POINT)
                    this->end_point = (Point*)obj;

                break;
            default:
                break;
        }

    }

}

//----------	getters and setters    ----------

double Line::getLength()
{
	return this->start_point->distanceFrom(end_point->getLocation());
}

Line *Line::setLength(double lenght)
{
	double multiplier = lenght / getLength();

	Vector2D old_vector = this->getLineVector();

	Vector2D new_vector(
				old_vector.x * multiplier,
				old_vector.y * multiplier
				);

	this->end_point
			->setLocation(this->start_point->getX() + new_vector.x,
						this->start_point->getY() + new_vector.y
						);

	return this;
}

Vector2D Line::getLineVector()
{
	this->line_vector = Vector2D(
				this->end_point->getX() - this->start_point->getX(),
				this->end_point->getY() - this->start_point->getY()
				);
	return this->line_vector;
}

Line *Line::setLineVector(Vector2D vector)
{
	double vec_conf = getLength()  / vector.length();
	this->line_vector.x = vec_conf *vector.x;
	this->line_vector.y = vec_conf *vector.y;

    this->end_point->setX(
                this->start_point->getY() + this->line_vector.x
                );
    this->end_point->setY(
                this->start_point->getY() + this->line_vector.y
                );

	return this;
}

Point *Line::getStartPoint()
{
	return this->start_point;
}

Point *Line::getEndPoint()
{
	return this->end_point;
}

Line *Line::Clone()
{
	Line *l = new Line(this->start_point, this->end_point);
	l->setName(this->getName());

	return l;
}


//----------    Geometry    ----------

double Line::getAngle(Vector2D *reference_vector)
{
	getLineVector();
	double scalar_mult = (
						this->line_vector.x * reference_vector->x
						+ this->line_vector.y * reference_vector->y
						);

	return qAcos(
				scalar_mult /
				this->getLength() * reference_vector->length()
				 );
}

Line *Line::setAngle(double angle, Vector2D *reference_vector)
{
	double angle_difference = angle - this->getAngle(reference_vector);

	this->end_point->setLocation(
				this->end_point->getX() * (qCos(angle_difference) - qSin(angle_difference)),
				this->end_point->getY() * (qSin(angle_difference) - qCos(angle_difference))
				);

	return this;
}

//----------     Distance    ----------

double Line::distanceFrom(Point *point)
{
	double denominator = abs(
				((this->end_point->getY() - this->start_point->getY()) * point->getY()) -
				((this->end_point->getX() - this->start_point->getX()) * point->getX()) +
				(this->end_point->getX() * this->start_point->getY()) -
				(this->end_point->getY() * this->start_point->getX())
				);
	double numerator = sqrt(
				pow(this->end_point->getY() - this->start_point->getY(), 2) +
				pow(this->end_point->getX() - this->start_point->getX(), 2)
				);

	return denominator / numerator;
}

//----------	QGraphicsItem overrides    ----------

QRectF Line::boundingRect() const
{
	QPointF first = this->start_point->getLocation();
	QPointF second = this->end_point->getLocation();

    return QRectF(first, second);
}

void Line::paint(QPainter *painter,
				 const QStyleOptionGraphicsItem *option,
				 QWidget *widget)
{
	if(this->hidden)
		return;

	resolveTies();
	painter->drawLine(this->start_point->getLocation(),
						this->end_point->getLocation()
					);
}


