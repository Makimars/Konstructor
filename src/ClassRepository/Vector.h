#ifndef VECTOR_H
#define VECTOR_H

#include <QtMath>

struct Vector2D{
	Vector2D(){};
	Vector2D(double x, double y){
		this->x = x;
		this->y = y;
	}
    double length(){
        return sqrt((x*x) + (y*y));
    }
	double x,y;
};

struct Vector3D{
	Vector3D(double x, double y, double z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	double x, y, z;
};

#endif // VECTOR_H
