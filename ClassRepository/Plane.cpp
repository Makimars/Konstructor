#include "Plane.h"

Plane::Plane()
{
    this->objects = new QVector<DrawableObject*>;

    /*
    //test code
    Point * p1 = addPoint(-100,-100);
    Point * p2 = addPoint(-100, 100);
    Point * p3 = addPoint(100, 100);
    Point * p4 = addPoint(100,-100);
    addLine(p1, p2);
    addLine(p2, p3);
    addLine(p3, p4);
    addLine(p4, p1);

    addCircle(p3, 50);
    Point * p5 = addPoint(1000,1000);
    Point * p6 = addPoint(1200,1200);
    addLine(p5,p6);
    addLine(p4, p5);
    */
}


//----------	object managment    ----------

DrawableObject * Plane::addDrawable(DrawableObject * obj)
{
    if(objects->lastIndexOf(obj) != -1)
        return nullptr;

    obj->setId(this->id_counter);
    this->id_counter++;

    this->objects->append(obj);

    return obj;
}

Point * Plane::addPoint(double x, double y)
{
    Point * p = new Point(x,y);
    addDrawable(p);
    return p;
}

Line * Plane::addLine(Point * start_point, Point * end_point)
{
    if(this->objects->lastIndexOf(start_point) == -1)
        this->addDrawable(start_point);
    if(this->objects->lastIndexOf(end_point) == -1)
        this->addDrawable(end_point);

    Line * ln = new Line(start_point, end_point);
    addDrawable(ln);
    return ln;
}

Circle * Plane::addCircle(Point * center, double radius)
{
    if(this->objects->lastIndexOf(center) == -1)
        this->addDrawable(center);

    Circle * c = new Circle(center, radius);
    addDrawable(c);
    return c;
}

