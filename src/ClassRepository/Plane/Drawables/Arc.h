#ifndef ARC_H
#define ARC_H

#include "Circle.h"

class Arc : public DrawableObject
{
public:
	Arc();
	Arc(Point *leftPoint, Point *rightPoint, Point *centerPoint);

	void resolveTies() override;

	//file handling
	void loadVariables(QString input) override;
	QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> list) override;

	double getRadius();

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   )override;

private:
	Point *centerPoint;
	Point *edgePoints[2];

	//geometry
	void setGeometryUpdates() override;
	void unsetGeometryUpdates() override;
};

#endif // ARC_H
