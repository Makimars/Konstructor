#ifndef PLANE_H
#define PLANE_H

#include "Drawables/Circle.h"

class Plane
{
public:
    Plane();

    QVector<DrawableObject*> * objects;


    //axes
    DrawableObject * up_up_axis, * up_down_axis, * down_down_axis, * down_up_axis;

    //object managment
    DrawableObject * addDrawable(DrawableObject * obj);
    Point * addPoint(double x, double y);
    Line * addLine(Point * start_point, Point * end_point);
    Circle * addCircle(Point * center, double radius);

private:
    unsigned int id_counter = 0;

};

#endif // PLANE_H
