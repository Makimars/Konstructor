#include "DrawablesFactory.h"

DrawablesFactory *DrawablesFactory::instance = nullptr;

void DrawablesFactory::initialise(QBrush *default_brush,
									QPen *default_pen,
									QVector<DrawableObject*> *object_list,
									QGraphicsScene *scene
									)
{
	if(DrawablesFactory::instance == nullptr)
	{
		DrawablesFactory::instance = new DrawablesFactory(default_brush,
														  default_pen,
														  object_list,
														  scene
														  );
	}
}

DrawablesFactory *DrawablesFactory::getInstance()
{
	return DrawablesFactory::instance;
}

//----------     object creation     ---------

Point *DrawablesFactory::makePoint()
{
	Point *point = new Point();
	point->setBrush(this->default_brush)
			->setPen(this->default_pen);

	return point;
}

Line *DrawablesFactory::makeLine(Point *start_point, Point *end_point)
{
	Line *line = new Line(start_point, end_point);
	line->setBrush(this->default_brush)
			->setPen(this->default_pen);

	return line;
}


Circle *DrawablesFactory::makeCircle(Point *center_point)
{
	Circle *circle = new Circle(center_point);
	circle->setBrush(this->default_brush)
			->setPen(this->default_pen);

	return circle;
}


Circle *DrawablesFactory::makeCircle(Point *center_point, double radius)
{
	Circle *circle = makeCircle(center_point);
	circle->setRadius(radius);

	return circle;
}

Circle *DrawablesFactory::makeCircle(Point *center_point, Point *lies_on)
{
	Circle *circle = makeCircle(center_point);
	circle->setRelationLiesOn(lies_on);

	return circle;
}

Circle *DrawablesFactory::makeCircle(Point *center_point, Line *lies_on)
{

}

Circle *DrawablesFactory::makeCircle(Point *center_point, DrawableObject *lies_on)
{

}

//----------     dimension creation     ---------

LineLenghtDimension *DrawablesFactory::makeDimension(Line *line, double lenght)
{

}

LinesAngleDimension *DrawablesFactory::makeDimension(Line *lines[], double angle)
{

}

LinesDistanceDimension *DrawablesFactory::makeDimension(Line *lines[])
{

}

//----------     object managment     ---------

void DrawablesFactory::addDrawable(DrawableObject *object)
{
	if(!this->object_list->contains(object))
	{
		this->object_list->append(object);
		this->scene->addItem(object);

		object->setId(this->id_counter);
		this->id_counter++;
	}
}

void DrawablesFactory::tryDeleteDrawable(DrawableObject *object)
{
	if(!this->object_list->contains(object))
		deleteDrawable(object);
}

void DrawablesFactory::deleteDrawable(DrawableObject *object)
{
	removeDrawable(object);

	if(object != nullptr)
	{
		delete object;
		object = nullptr;
	}
}

void DrawablesFactory::removeDrawable(DrawableObject *object)
{
	if(object_list->contains(object))
		this->object_list->removeAll(object);
	this->scene->removeItem(object);
}


//----------     _     ---------

DrawablesFactory::DrawablesFactory(QBrush *default_brush,
								   QPen *default_pen,
								   QVector<DrawableObject*> *object_list,
								   QGraphicsScene *scene
								   )
{
	this->default_brush = default_brush;
	this->default_pen = default_pen;
	this->object_list = object_list;
	this->scene = scene;
}
