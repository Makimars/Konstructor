#ifndef LINE_H
#define LINE_H

#include "Point.h"

#define TYPE_LINE "Line"

class Line : public DrawableObject
{
public:
	Line();
	Line(Point *startPoint, Point *endPoint);

    //file handling
    void fromFileString(QString json) override;
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
	QVector2D getLineVector();
	Line *setLineVector(QVector2D vector);

	/**
	 * @brief gets angle of this line relative to reference vector in radians
	 * @param Vector2D *referenceVector
	 * @return double angle (radians)
	 */
	double getAngle(QVector2D referenceVector);
	/**
	 * @brief sets the angle in radians of this line relative to reference vector
	 * @param double angle (radians)
	 * @param Vector2D *referenceVector
	 * @return this
	 */
	Line *setAngle(double angle, QVector2D referenceVector);

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
	QVector2D lineVector;

	//defining variables
	Point *startPoint, *endPoint;
};

#endif // LINE_H
