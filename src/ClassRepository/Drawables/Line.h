#ifndef LINE_H
#define LINE_H

#include "Point.h"

class Line : public DrawableObject
{
public:
	Line();
	Line(Point *startPoint, Point *endPoint);
	~Line() override;

    //file handling
    void loadVariables(QString json) override;
    QString toFileString() override;
    void loadRelations(QVector<DrawableObject*> *list) override;

	//getters and setters
	double getLength();
	Line  *setLength(float lenght);
	bool isInView(QGraphicsView *view, Point *origin, double scale);
	Point *getStartPoint();
	Point *getEndPoint();
	Line *clone() override;

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

protected:
	//defining variables
	Point *startPoint, *endPoint;

	//geometry
	void setGeometryUpdates();
};

#endif // LINE_H
