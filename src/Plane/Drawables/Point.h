#ifndef POINT_H
#define POINT_H

#include "DrawableObject.h"

class Point : public DrawableObject
{
public:
	Point();
	Point(double x, double y);

    //file handling
	void loadData(nlohmann::json jsonInput) override;
	nlohmann::json toJson() override;

    //getters and setters
	double getX() const;
	void setX(double x);
	double getY() const;
	void setY(double y);
	QPointF getLocation() const;
	void setLocation(QPointF point);
	void setLocation(double x, double y);

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
inline double Point::getX() const { return x; }
inline void Point::setX(double x) { this->x = x; }
inline double Point::getY() const { return y; }
inline void Point::setY(double y) { this->y = y; }
inline QPointF Point::getLocation() const { return QPointF(x, y); }
inline void Point::setLocation(QPointF point) { setLocation(point.x(), point.y()); }

#endif // POINT_H
