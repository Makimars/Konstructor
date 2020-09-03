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
    void loadVariables(QString input) override;
	QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> list) override;

	//getters and setters
	Point *getCenterPoint();
	double getRadius();
	Circle *setRadius(double value);
	Circle *clone() override;

	//relations
	void setRelationLiesOn(Point *point);

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
	Point *liesOn = nullptr;

	//geometry
	void setGeometryUpdates() override;
	void unsetGeometryUpdates() override;

	//events
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

//inline getters and setters
inline Point *Circle::getCenterPoint() { return centerPoint; }
inline double Circle::getRadius() { return radius; }

#endif // CIRCLE_H
