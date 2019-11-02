#include "Tool.h"

void Tool::resetTool()
{

}

Tool::Tool()
{

}

Point *Tool::addPoint(double x, double y)
{
	Point *p = new Point(x,y);
	emit addDrawable(p);

	return p;
}

Point *Tool::addPoint(QPointF location)
{
	return addPoint(location.x(), location.y());
}

Line *Tool::addLine(Point *start_point, Point *end_point)
{
	emit addDrawable(start_point);
	emit addDrawable(end_point);

	Line *ln = new Line(start_point, end_point);
	emit addDrawable(ln);

	return ln;
}

Circle *Tool::addCircle(Point *center, double radius)
{
	emit addDrawable(center);

	Circle *c = new Circle(center, radius);
	emit addDrawable(c);

	return c;
}

Circle *Tool::addCircle(Point *center, Point *lies_on)
{
	emit addDrawable(center);

	Circle *c = new Circle(center, lies_on);
	emit addDrawable(c);

	return c;
}
