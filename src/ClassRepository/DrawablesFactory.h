#ifndef DRAWABLESFACTORY_H
#define DRAWABLESFACTORY_H

#include "Drawables/Dimensions/LinesDistanceDimension.h"

class DrawablesFactory
{
public:
	static void initialise(QBrush *defaultBrush,
							QPen *defaultPen,
							QVector<DrawableObject*> *objectList,
							QGraphicsScene *scene
							);
	static DrawablesFactory *getInstance();

	//object creation
	Point *makePoint();
	Line *makeLine(Point *startPoint, Point *endPoint);
	Circle *makeCircle(Point *centerPoint);
	Circle *makeCircle(Point *centerPoint, double radius);
	Circle *makeCircle(Point *centerPoint, DrawableObject *liesOn);

	//dimension creation
	LineLengthDimension *makeLineLengthDimension(Line *line, double lenght);
	LineLengthDimension *makeLineLengthDimension(Line *line, double lenght, double distanceFromLine);
	LinesAngleDimension *makeLinesAngleDimension(Line *lines[2], double angle);
	LinesDistanceDimension *makeLinesDistanceDimension(Line *lines[2]);

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
	DrawablesFactory(QBrush *defaultBrush,
					 QPen *defaultPen,
					 QVector<DrawableObject*> *objectList,
					 QGraphicsScene *scene
					 );

	unsigned int idCounter = 0;

	QVector<DrawableObject*> *objectList;
	QGraphicsScene *scene;

	QPen *defaultPen;
	QBrush *defaultBrush;



};

#endif // DRAWABLESFACTORY_H
