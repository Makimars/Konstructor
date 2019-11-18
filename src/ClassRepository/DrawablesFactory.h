#ifndef DRAWABLESFACTORY_H
#define DRAWABLESFACTORY_H

#include "Drawables/Dimensions/LinesDistanceDimension.h"

class DrawablesFactory
{
public:
	static void initialise(QBrush *default_brush,
							QPen *default_pen,
							QVector<DrawableObject*> *object_list,
							QGraphicsScene *scene
							);
	static DrawablesFactory *getInstance();

	//object creation
	Point *makePoint();
	Line *makeLine(Point *start_point, Point *end_point);
	Circle *makeCircle(Point *center_point);
	Circle *makeCircle(Point *center_point, double radius);
	Circle *makeCircle(Point *center_point, Point *lies_on);
	Circle *makeCircle(Point *center_point, Line *lies_on);
	Circle *makeCircle(Point *center_point, DrawableObject *lies_on);

	//dimension creation
	LineLenghtDimension *makeDimension(Line *line, double lenght);
	LinesAngleDimension *makeDimension(Line *lines[2], double angle);
	LinesDistanceDimension *makeDimension(Line *lines[2]);

	//object managment
	/**
	 * @brief adds drawable to object list and scene
	 */
	void addDrawable(DrawableObject *object);

	/**
	 * @brief deletes drawable if it is not in object list
	 */
	void tryDeleteDrawable(DrawableObject *object);
	/**
	 * @brief removeDrawable() and deletes it
	 */
	void deleteDrawable(DrawableObject *object);
	/**
	 * @brief removes drawable from object list and scene
	 */
	void removeDrawable(DrawableObject *object);


private:
	static DrawablesFactory *instance;
	DrawablesFactory(QBrush *default_brush,
					 QPen *default_pen,
					 QVector<DrawableObject*> *object_list,
					 QGraphicsScene *scene
					 );

	unsigned int id_counter = 0;

	QVector<DrawableObject*> *object_list;
	QGraphicsScene *scene;

	QPen *default_pen;
	QBrush *default_brush;



};

#endif // DRAWABLESFACTORY_H
