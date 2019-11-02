#ifndef TOOL_H
#define TOOL_H

#include "../Drawables/Circle.h"

class Tool : public QObject
{
	Q_OBJECT
public:
	virtual void click(Point * clicked_point, bool existing_point = false){}
	virtual void resetTool();

private:

protected:
	Tool();

	Point *addPoint(double x, double y);
	Point *addPoint(QPointF location);
	Line *addLine(Point *start_point, Point *end_point);
	Circle *addCircle(Point *center, double radius);
	Circle *addCircle(Point *center, Point *lies_on);

signals:
	void addDrawable(DrawableObject *object);
	void tryDeleteDrawable(DrawableObject *object);

};

#endif // TOOL_H
