#ifndef DRAWABLESFACTORY_H
#define DRAWABLESFACTORY_H

#include "QGraphicsViewUserInput.h"
#include "src/ClassRepository/Exceptions/DrawableAlreadyRestrainedException.h"

class DrawablesFactory
{
public:
	static void initialise(QBrush *defaultBrush,
							QPen *defaultPen,
							QVector<DrawableObject*> *objectList,
							QVector<DrawableObject*> *staticObjectsList,
							QGraphicsScene *scene
							);
	static DrawablesFactory *getInstance();

	//object creation
	Point *makePoint();
	Point *makePoint(double x, double y);
	Line *makeLine(Point *startPoint, Point *endPoint);
	Circle *makeCircle(Point *centerPoint);
	Circle *makeCircle(Point *centerPoint, double radius);
	Circle *makeCircle(Point *centerPoint, Point *liesOn);
	Label *makeLabel(QPointF location);
	Label *makeLabel(QPointF location, QString text);

	//dimension creation
	LineLengthDimension *makeLineLengthDimension(Line *line);
	LineLengthDimension *makeLineLengthDimension(Line *line, double lenght);
	LineLengthDimension *makeLineLengthDimension(Line *line, double lenght, double distanceFromLine);
	LinesAngleDimension *makeLinesAngleDimension(Line *lines[2]);
	LinesAngleDimension *makeLinesAngleDimension(Line *lines[2], double distanceFromCenter);
	CircleRadiusDimension *makeCircleRadiusDimension(Circle *circle);
	CirclesRadiusDifferenceDimension *makeCirclesRadiusDifferenceDimension(Circle *circles[2]);

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
	/**
	 * @brief deletes all objects and resets the id counter to 0
	 */
	void deleteAll();

private:
	static DrawablesFactory *instance;
	DrawablesFactory(QBrush *defaultBrush,
					 QPen *defaultPen,
					 QVector<DrawableObject*> *objectList,
					 QVector<DrawableObject*> *staticObjectsList,
					 QGraphicsScene *scene
					 );

	int idCounter = 0;

	QVector<DrawableObject*> *objectList;
	QVector<DrawableObject*> *staticObjectsList;
	QGraphicsScene *scene;

	QPen *defaultPen;
	QBrush *defaultBrush;

	QGraphicsViewUserInput *userInput;

};

#endif // DRAWABLESFACTORY_H
