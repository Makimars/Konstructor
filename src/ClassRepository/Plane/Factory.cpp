#include "Factory.h"

Factory *Factory::instance = nullptr;

void Factory::initialise(const Style *defaultStyle,
									QVector<DrawableObject*> *objectList,
									QVector<DrawableObject*> *staticObjectsList,
									QGraphicsScene *scene
									)
{
	if(Factory::instance == nullptr)
	{
		Factory::instance = new Factory(defaultStyle, objectList, staticObjectsList, scene);
	}
}

Factory *Factory::getInstance()
{
	return Factory::instance;
}

//----------     object creation     ---------

Point *Factory::makePoint()
{
	Point *point = new Point();
	point->setStyle(currentStyle);

	return point;
}

Point *Factory::makePoint(double x, double y)
{
	Point *point = makePoint();
	point->setLocation(x, y);

	return point;
}

Point *Factory::copyPoint(const Point *point)
{
	Point *newPoint = makePoint();
	newPoint->setLocation(point->getLocation());

	return newPoint;
}

Line *Factory::makeLine(Point *startPoint, Point *endPoint)
{
	Line *line = new Line(startPoint, endPoint);
	line->setStyle(currentStyle);

	return line;
}

Circle *Factory::makeCircle(Point *centerPoint)
{
	Circle *circle = new Circle(centerPoint);
	circle->setStyle(currentStyle);

	return circle;
}


Circle *Factory::makeCircle(Point *centerPoint, double radius)
{
	Circle *circle = makeCircle(centerPoint);
	circle->setRadius(radius);

	return circle;
}

Circle *Factory::makeCircle(Point *centerPoint, Point *liesOn)
{
	Circle *circle = makeCircle(centerPoint);
	circle->setRelationLiesOn(liesOn);

	return circle;
}

Label *Factory::makeLabel(QPointF location)
{
	Label *label = new Label(location);
	label->setStyle(currentStyle);

	QObject::connect(label, &UserInputRequester::requestString,
					 this->userInput, &QGraphicsViewUserInput::requestString
					 );

	return label;
}

Label *Factory::makeLabel(QPointF location, QString text)
{
	Label *label = new Label(location, text);
	label->setStyle(currentStyle);

	QObject::connect(label, &UserInputRequester::requestString,
					 this->userInput, &QGraphicsViewUserInput::requestString
					 );

	return label;
}

Arc *Factory::makeArc(Point *points[])
{

}

//----------     dimension creation     ---------

LineLengthDimension *Factory::makeLineLengthDimension(Line *line)
{
	LineLengthDimension *dimension = new LineLengthDimension(line);
	dimension->setStyle(currentStyle);

	QObject::connect(dimension, &UserInputRequester::requestDouble,
					 this->userInput, &QGraphicsViewUserInput::requestDouble
					 );

	return dimension;
}

LineLengthDimension *Factory::makeLineLengthDimension(Line *line, double length)
{
	LineLengthDimension *dimension = makeLineLengthDimension(line);
	dimension->setLineLength(length);

	return dimension;
}

LineLengthDimension *Factory::makeLineLengthDimension(Line *line, double length, double distanceFromLine)
{
	LineLengthDimension *dimension = makeLineLengthDimension(line, length);
	dimension->setDistanceFromLine(distanceFromLine);

	return dimension;
}

LinesAngleDimension *Factory::makeLinesAngleDimension(Line *lines[])
{
	LinesAngleDimension *dimension = new LinesAngleDimension(lines);
	dimension->setStyle(currentStyle);

	QObject::connect(dimension, &UserInputRequester::requestDouble,
					 this->userInput, &QGraphicsViewUserInput::requestDouble
					 );

	return dimension;
}

LinesAngleDimension *Factory::makeLinesAngleDimension(Line *lines[], double distanceFromCenter)
{
	LinesAngleDimension *dimension = makeLinesAngleDimension(lines);
	dimension->setDistanceFromCenter(distanceFromCenter);

	return dimension;
}

CircleRadiusDimension *Factory::makeCircleRadiusDimension(Circle *circle)
{
	CircleRadiusDimension *dimension = new CircleRadiusDimension(circle);
	dimension->setStyle(currentStyle);

	QObject::connect(dimension, &UserInputRequester::requestDouble,
					 this->userInput, &QGraphicsViewUserInput::requestDouble
					 );

	return dimension;
}

CirclesRadiusDifferenceDimension *Factory::makeCirclesRadiusDifferenceDimension(Circle *circles[])
{
	CirclesRadiusDifferenceDimension *dimension = new CirclesRadiusDifferenceDimension(circles);
	dimension->setStyle(currentStyle);

	QObject::connect(dimension, &UserInputRequester::requestDouble,
					 this->userInput, &QGraphicsViewUserInput::requestDouble
					 );

	return dimension;
}

void Factory::addToScene(DrawableObject *object)
{
	object->setStyle(currentStyle);

	scene->addItem(object);
}

//----------     object managment     ---------

void Factory::addDrawable(DrawableObject *object)
{
	object->setStyle(currentStyle);

	if(!this->objectList->contains(object) & !this->staticObjectsList->contains(object))
	{
		object->setFlags(QGraphicsItem::ItemIsSelectable);
		object->setAcceptHoverEvents(true);
		this->objectList->append(object);
		this->scene->addItem(object);

		object->setId(this->idCounter);
		this->idCounter++;
	}
}

void Factory::tryDeleteDrawable(DrawableObject *object)
{
	if(!this->objectList->contains(object) & !this->staticObjectsList->contains(object))
		deleteDrawable(object);
}

void Factory::deleteDrawable(DrawableObject *object)
{
	if(object != nullptr)
	{
		removeDrawable(object);

		delete object;
		object = nullptr;
	}
}

void Factory::removeDrawable(DrawableObject *object)
{
	object->removeGeometryUpdates();
	for(int i = 0; i < this->objectList->size(); i++)
	{
		this->objectList->at(i)->removeGeometryUpdate(object);
	}

	if(objectList->contains(object))
		this->objectList->removeAll(object);
	if(this->scene->items().contains(object))
		this->scene->removeItem(object);
}

void Factory::deleteAll()
{
	foreach(DrawableObject *item, *this->objectList)
	{
		delete item;
	}
	objectList->clear();
	this->idCounter = 0;
}


//----------     _     ---------

Factory::Factory(const Style *defaultStyle,
					QVector<DrawableObject*> *objectList,
					QVector<DrawableObject*> *staticObjectsList,
					QGraphicsScene *scene)
{
	this->currentStyle = defaultStyle;
	this->objectList = objectList;
	this->staticObjectsList = staticObjectsList;
	this->scene = scene;

	QGraphicsViewUserInput::initialize(scene);
	this->userInput = QGraphicsViewUserInput::getInstance();
}
