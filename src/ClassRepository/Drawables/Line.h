#ifndef LINE_H
#define LINE_H

#include "Point.h"

#define TYPE_LINE "Line"

class Line : public DrawableObject
{
public:
	Line();
	Line(Point *start_point, Point *end_point);
    ~Line() override;

	void resolveTies() override;

    //file handling
    void fromFileString(QString json) override;
    QString toFileString() override;
    void loadRelations(QVector<DrawableObject*> *list) override;

	//getters and setters
	double getLength();
	Line  *setLength(double lenght);
	bool isInView(QGraphicsView *view, Point *origin, double scale);
	Point *getStartPoint();
	Point *getEndPoint();
	Line *Clone() override;

	//Geometry
	Vector2D getLineVector();
	Line *setLineVector(Vector2D vector);

	/**
	 * @brief gets angle of this line relative to reference vector in radians
	 * @param Vector2D *reference_vector
	 * @return double angle (radians)
	 */
	double getAngle(Vector2D *reference_vector);
	/**
	 * @brief sets the angle in radians of this line relative to reference vector
	 * @param double angle (radians)
	 * @param Vector2D *reference_vector
	 * @return this
	 */
	Line *setAngle(double angle, Vector2D *reference_vector);

	//Distance
	double distanceFrom(Point *point);

    //QGraphicsItem overrides
	QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget
               ) override;

protected:
    Vector2D line_vector;

    //plane axes
	Line *up_up_axis;

	//defining variables
	Point *start_point, *end_point;
};

#endif // LINE_H
