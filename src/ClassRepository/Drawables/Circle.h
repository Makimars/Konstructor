#ifndef CIRCLE_H
#define CIRCLE_H

#include "Line.h"

class Circle : public DrawableObject
{
public:
	Circle();
	Circle(Point *centerPoint);

	void resolveTies() override;

	//file handling
    void fromFileString(QString input) override;
	QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> *list) override;

	//getters and setters
	Point *getCenterPoint();
	double *getRadius();
	Circle *setRadius(double value);
	Circle *clone() override;

	//relations
	void setRelationLiesOn(DrawableObject *point);

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   )override;

private:
	//defining variables
	Point *centerPoint;
	double radius;

	//relations
	DrawableObject *liesOn = nullptr;

};

#endif // CIRCLE_H
