#ifndef DRAWABLESFACTORY_H
#define DRAWABLESFACTORY_H

#include "Drawables/Dimensions/CirclesRadiusDifferenceDimension.h"

#include "QGraphicsViewUserInput.h"
#include "Drawables/Constraints/Constraints.h"

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
	Label *makeLabel(QPointF location);
	// 0 and 1 edge points, 2 center
	Arc *makeArc(Point *points[3]);

	//constrains
	PointDistanceConstraint *makePointDistanceConstraint(Point *originPoint, Point *drivenPoint);
	CircleRadiusConstraint *makeCircleRadiusConstraint(Circle *circle);
	LineCenterPointConstraint *makeLineCenterPointConstraint(Line *line, Point *point);
	ParaelLinesConstraint *makeParaelLinesConstraint(Line *lineOne, Line *lineTwo);

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
	 * @brief removes drawable from object list and scene and deletes it
	 */
	bool deleteDrawable(DrawableObject *object);
	/**
	 * @brief deletes all objects and resets the id counter to 1
	 */
	void deleteAll();

	void addStaticDrawable(DrawableObject *object);
	void deleteAllStaticDrawables();

	QVector<DrawableObject*> generateListFromSketch(QString sketch);

private:
	static Factory *instance;
	Factory(const Style *defaultStyle,
				QVector<DrawableObject*> *objectList,
				QVector<DrawableObject*> *staticObjectsList,
				QGraphicsScene *scene
				);

	uint32_t idCounter = 1;
	int staticIdCounter = -1;

	QVector<DrawableObject*> *objectList;
	QVector<DrawableObject*> *staticObjectsList;
	QGraphicsScene *scene;

	const Style *currentStyle;

	QGraphicsViewUserInput *userInput;

};

#endif // DRAWABLESFACTORY_H
