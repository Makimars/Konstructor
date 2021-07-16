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
	void loadData(nlohmann::json jsonInput) override;
	void loadRelations(QVector<DrawableObject*> list) override;
	nlohmann::json toJson() override;

	//getters and setters
	Point *getCenterPoint();
	double getRadius();
	Circle *setRadius(double value);

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
