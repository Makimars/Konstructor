#ifndef LINE_H
#define LINE_H

#include "DrawableObject.h"
#include "Point.h"
#include "ClassRepository/Vector.h"

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
    void loadRelations(QVector<DrawableObject> *list) override;

    //getters and setters
	Vector2D *getLineVector();
	void setLineVector(Vector2D vector);
	bool isInView(QGraphicsView *view, Point *origin, double scale);
	Point * getStartPoint();
	Point * getEndPoint();

    //QGraphicsItem overrides
	QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget
               ) override;

private:
	Point *start_point, *end_point;
	Vector2D line_vector;
};

#endif // LINE_H
