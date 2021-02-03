#ifndef LINE_H
#define LINE_H

#include "Point.h"

class Line : public DrawableObject
{
public:
	Line();
	Line(Point *startPoint, Point *endPoint);

    //file handling
    void loadVariables(QString json) override;
    QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> list) override;

	//getters and setters
	double getLength();
	void setLength(float lenght);
	Point *getStartPoint();
	Point *getEndPoint();

	//Geometry
	QVector2D getLineVector() const;
	Line *setLineVector(QVector2D vector);

	//Distance
	double distanceFrom(QPointF location);
	double signedDistanceFrom(QPointF location);

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   ) override;

	//static distance
	static double distanceFrom(QPointF p0, QPointF p1, QPointF location);
	static double signedDistanceFrom(QPointF p0, QPointF p1, QPointF location);

private:
	//defining variables
	Point *startPoint, *endPoint;

	//geometry
	void setGeometryUpdates() override;
	void unsetGeometryUpdates() override;
};

//inline getters and setters
inline double Line::getLength() { return startPoint->distanceFrom(endPoint->getLocation()); }

#endif // LINE_H
