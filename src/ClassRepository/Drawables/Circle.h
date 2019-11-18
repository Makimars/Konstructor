#ifndef CIRCLE_H
#define CIRCLE_H

#include "Line.h"

#define TYPE_CIRCLE "Circle"

class Circle : public DrawableObject
{
public:
	Circle();
	Circle(Point *center_point);
    ~Circle() override;

	void resolveTies() override;

	//file handling
    void fromFileString(QString input) override;
	void loadRelations(QVector<DrawableObject*> *list) override;
    QString toFileString() override;

	//getters and setters
	Point *getCenterPoint();
	double *getRadius();
	Circle *setRadius(double value);
	Circle *Clone() override;

	//relations
	void setRelationLiesOn(Point *p);

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   )override;

private:
	//defining variables
	Point *center_point;
	double radius;

	//relations
	Point *lies_on;

};

#endif // CIRCLE_H
