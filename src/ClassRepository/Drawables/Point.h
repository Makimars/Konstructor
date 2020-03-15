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
	Point *setLocation(QPointF point);
	Point *setLocation(double x, double y);
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

#endif // POINT_H
