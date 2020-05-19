#ifndef POINT_H
#define POINT_H

#include "DrawableObject.h"

class Point : public DrawableObject
{
public:
	Point();

    //file handling
    void loadVariables(QString json) override;
	QString toFileString() override;

    //getters and setters
	double getX();
	double getY();
	QPointF getLocation();
	void setLocation(QPointF point);
	void setLocation(double x, double y);
	Point *clone() override;

    //aritmetic functions
	double distanceFrom(QPointF point);

    //QGraphicsItem overrides
    QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
               )override;

private:
	//defining variables
	double x, y;

	//events
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

//inline getters and setters
inline double Point::getX() { return x; }
inline double Point::getY() { return y; }
inline QPointF Point::getLocation() { return QPointF(x, y); }
inline void Point::setLocation(QPointF point) { setLocation(point.x(), point.y()); }

#endif // POINT_H
