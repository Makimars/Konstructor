#ifndef LINE_H
#define LINE_H

#include "Point.h"

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
    double lenght();
	bool isInView(QGraphicsView *view, Point *origin, double scale);
	Point *getStartPoint();
	Point *getEndPoint();
	Line *Clone() override;

    //Geonmetry
	Vector2D *getLineVector();
    void setLineVector(Vector2D vector);
	double getAngle(Line *reference_line);
	void setAngle(double angle, Line *reference_line);

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
