#include "Circle.h"

Circle::Circle()
{
	this->type = TYPE_CIRCLE;
}

Circle::Circle(Point *center_point)
{
	this->type = TYPE_CIRCLE;
	this->centerPoint = center_point;
}

void Circle::resolveTies()
{
	if(this->liesOn != nullptr)
	{
		if(this->liesOn->getType() == TYPE_POINT)
		{
			Point *liesOn = dynamic_cast<Point*>(this->liesOn);

			this->radius = this->centerPoint->distanceFrom(liesOn->getLocation());
		}
	}
}

//----------	file handling    ----------

void Circle::fromFileString(QString input)
{
	DrawableObject::fromFileString(input);

	QStringList varNames = {
      "radius"
    };

	QStringList variables = input.split(',');
    for(int i = 0; i < variables.length() - 1; i++)
	{
		QStringList parts = variables[i].split(":");
		QString varName = parts[0];
		QString varValue = parts[1];

		switch (varNames.indexOf(varName)) {
			case 0:
				this->radius = QVariant(varValue).toDouble();
				break;
			default:
				break;
		}

	}
}

void Circle::loadRelations(QVector<DrawableObject*> *list)
{
	QStringList varNames = {
		"centerPoint"
    };

    QStringList variables = this->file.split(',');
    for(int i = 0; i < variables.length() - 1; i++)
    {
        QStringList parts = variables[i].split(":");
		QString varName = parts[0];
		QString varValue = parts[1];

		DrawableObject *obj;

		switch (varNames.indexOf(varName)) {
            case 0:
				obj = DrawableObject::getById(list, QVariant(varValue).toUInt());
				if(obj->getType() == TYPE_POINT)
					this->centerPoint = dynamic_cast<Point*>(obj);

                break;
            default:
                break;
        }

    }
}

QString Circle::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("centerPoint", this->centerPoint->getId());
	this->fileAddVar("radius", this->radius);
	return this->fileFinish();
}

//----------	getters and setters    ----------

Point *Circle::getCenterPoint()
{
	return this->centerPoint;
}

double *Circle::getRadius()
{
	return &this->radius;
}

Circle *Circle::setRadius(double value)
{
	this->radius = value;

	return this;
}

Circle *Circle::clone()
{
	Circle *circle = new Circle(this->centerPoint);
	circle->setRadius(this->radius);
	circle->setRelationLiesOn(this->liesOn);

	return circle;
}

//----------	Relations    ----------

void Circle::setRelationLiesOn(DrawableObject *object)
{
	this->liesOn = object;
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
	if(this->hidden)
		return;

	DrawableObject::paint(painter, option, widget);

	painter->drawEllipse(this->boundingRect());
}
