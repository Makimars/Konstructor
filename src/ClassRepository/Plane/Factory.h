#ifndef DRAWABLESFACTORY_H
#define DRAWABLESFACTORY_H

#include "QGraphicsViewUserInput.h"
#include "src/ClassRepository/Exceptions/DrawableAlreadyRestrainedException.h"

class Factory
{
public:
	static void initialise(const Style *defaultStyle,
							QVector<DrawableObject*> *objectList,
							QVector<DrawableObject*> *staticObjectsList,
							QGraphicsScene *scene
							);
	static Factory *getInstance();

	QBrush *getCurrentBrush();
	QPen *getCurrentPen();

	//object creation
	Point *makePoint();
	Point *makePoint(double x, double y);
	Point *copyPoint(const Point *point);
	Line *makeLine(Point *startPoint, Point *endPoint);
	Circle *makeCircle(Point *centerPoint);
	Circle *makeCircle(Point *centerPoint, double radius);
	Circle *makeCircle(Point *centerPoint, Point *liesOn);
	Label *makeLabel(QPointF location);
	Label *makeLabel(QPointF location, QString text);
	// 0 and 1 edge points, 2 center
	Arc *makeArc(Point *points[3]);

	//dimension creation
	LineLengthDimension *makeLineLengthDimension(Line *line);
	LineLengthDimension *makeLineLengthDimension(Line *line, double lenght);
	LineLengthDimension *makeLineLengthDimension(Line *line, double lenght, double distanceFromLine);
	LinesAngleDimension *makeLinesAngleDimension(Line *lines[2]);
	LinesAngleDimension *makeLinesAngleDimension(Line *lines[2], double distanceFromCenter);
	CircleRadiusDimension *makeCircleRadiusDimension(Circle *circle);
	CirclesRadiusDifferenceDimension *makeCirclesRadiusDifferenceDimension(Circle *circles[2]);

	//object managment
	void addToScene(DrawableObject *object);
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

	QVector<DrawableObject*> generateListFromSketch(QString sketch);

private:
	static Factory *instance;
	Factory(const Style *defaultStyle,
				QVector<DrawableObject*> *objectList,
				QVector<DrawableObject*> *staticObjectsList,
				QGraphicsScene *scene
				);

	int idCounter = 0;

	QVector<DrawableObject*> *objectList;
	QVector<DrawableObject*> *staticObjectsList;
	QGraphicsScene *scene;

	const Style *currentStyle;

	QGraphicsViewUserInput *userInput;

};

#endif // DRAWABLESFACTORY_H
