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

void Line::loadRelations(QVector<DrawableObject*> * list)
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

        DrawableObject * obj;

        switch (var_names.indexOf(var_name)) {
            case 0:
                obj = DrawableObject::getById(list, QVariant(var_value).toUInt());
                if(obj->getType() == "Point")
                    this->start_point = (Point*)obj;

                break;
            case 1:
                obj = DrawableObject::getById(list, QVariant(var_value).toUInt());
                if(obj->getType() == "Point")
                    this->end_point = (Point*)obj;

                break;
            default:
                break;
        }

    }

}

//----------	getters and setters    ----------

Vector2D * Line::getLineVector()
{
    return &this->line_vector;
}

void Line::setLineVector(Vector2D vector)
{
    double vec_conf = lenght()  / vector.lenght();
    this->line_vector.x = vec_conf * vector.x;
    this->line_vector.y = vec_conf * vector.y;

    this->end_point->setX(
                this->start_point->getY() + this->line_vector.x
                );
    this->end_point->setY(
                this->start_point->getY() + this->line_vector.y
                );

}

double Line::lenght()
{
    return this->start_point->distanceFrom(end_point->getLocation());
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


