#ifndef CIRCLE_H
#define CIRCLE_H

#include "Line.h"

class Circle : public DrawableObject
{
public:
	Circle();
	Circle(Point *center_point, double radius);
    ~Circle() override;

	void resolveTies() override;

	//file handling
    void fromFileString(QString input) override;
	void loadRelations(QVector<DrawableObject*> *list) override;
    QString toFileString() override;

	//getters and setters
	Point *getCenterPoint();
	double *getRadius();
	void setRadius(double value);

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   )override;

private:
	Point *center_point;
	double radius;

};

#endif // CIRCLE_H
