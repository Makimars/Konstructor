#ifndef POINT_H
#define POINT_H

#include "DrawableObject.h"

class Point : public DrawableObject
{
public:
	Point();
    Point(QPointF location);
	Point(double x, double y);
    ~Point() override;

	void resolveTies() override;

    //file handling
    void fromFileString(QString json) override;
    QString toFileString() override;
    void loadRelations(QVector<DrawableObject*> *list) override;

    //getters and setters
	double getX();
	double getY();
	QPointF getLocation();
	void setLocation(QPointF point);
	void setLocation(double x, double y);

    //aritmetic functions
	double distanceFrom(QPointF point);

    //QGraphicsItem overrides
    QRectF boundingRect() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
               )override;

private:
	double x, y;

};

#endif // POINT_H
