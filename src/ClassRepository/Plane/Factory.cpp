#include "Factory.h"

Factory *Factory::instance = nullptr;

void Factory::initialise(QBrush *defaultBrush,
									QPen *defaultPen,
									QVector<DrawableObject*> *objectList,
									QVector<DrawableObject*> *staticObjectsList,
									QGraphicsScene *scene
									)
{
	if(Factory::instance == nullptr)
	{
		Factory::instance = new Factory(defaultBrush,
														  defaultPen,
														  objectList,
														  staticObjectsList,
														  scene
														  );
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
	point->setBrush(this->defaultBrush);
	point->setPen(this->defaultPen);

	return point;
}

Point *Factory::makePoint(double x, double y)
{
	Point *point = makePoint();
	point->setLocation(x, y);

	return point;
}

Line *Factory::makeLine(Point *startPoint, Point *endPoint)
{
	Line *line = new Line(startPoint, endPoint);
	line->setBrush(this->defaultBrush);
	line->setPen(this->defaultPen);

	return line;
}

Circle *Factory::makeCircle(Point *centerPoint)
{
	Circle *circle = new Circle(centerPoint);
	circle->setBrush(this->defaultBrush);
	circle->setPen(this->defaultPen);

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
	label->setPen(this->defaultPen);
	label->setBrush(this->defaultBrush);

	QObject::connect(label, &UserInputRequester::requestString,
					 this->userInput, &QGraphicsViewUserInput::requestString
					 );

	return label;
}

Label *Factory::makeLabel(QPointF location, QString text)
{
	Label *label = new Label(location, text);
	label->setPen(this->defaultPen);
	label->setBrush(this->defaultBrush);

	QObject::connect(label, &UserInputRequester::requestString,
					 this->userInput, &QGraphicsViewUserInput::requestString
					 );

	return label;
}

//----------     dimension creation     ---------

LineLengthDimension *Factory::makeLineLengthDimension(Line *line)
{
	LineLengthDimension *dimension = new LineLengthDimension(line);
	dimension->setPen(this->defaultPen);
	dimension->setBrush(this->defaultBrush);

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
	dimension->setPen(this->defaultPen);
	dimension->setBrush(this->defaultBrush);

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
	dimension->setPen(this->defaultPen);
	dimension->setBrush(this->defaultBrush);

	QObject::connect(dimension, &UserInputRequester::requestDouble,
					 this->userInput, &QGraphicsViewUserInput::requestDouble
					 );

	return dimension;
}

CirclesRadiusDifferenceDimension *Factory::makeCirclesRadiusDifferenceDimension(Circle *circles[])
{
	CirclesRadiusDifferenceDimension *dimension = new CirclesRadiusDifferenceDimension(circles);
	dimension->setPen(this->defaultPen);
	dimension->setBrush(this->defaultBrush);

	QObject::connect(dimension, &UserInputRequester::requestDouble,
					 this->userInput, &QGraphicsViewUserInput::requestDouble
					 );

	return dimension;
}

void Factory::addToScene(DrawableObject *object)
{
	object->setPen(this->defaultPen);
	object->setBrush(this->defaultBrush);

	scene->addItem(object);
}

//----------     object managment     ---------

void Factory::addDrawable(DrawableObject *object)
{
	object->setPen(this->defaultPen);
	object->setBrush(this->defaultBrush);

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
		item->removeGeometryUpdates();
	}
	foreach(DrawableObject *item, *this->objectList)
	{
		deleteDrawable(item);
	}
	this->idCounter = 0;
}


//----------     _     ---------

Factory::Factory(QBrush *defaultBrush,
								   QPen *defaultPen,
								   QVector<DrawableObject*> *objectList,
								   QVector<DrawableObject*> *staticObjectsList,
								   QGraphicsScene *scene
								   )
{
	this->defaultBrush = defaultBrush;
	this->defaultPen = defaultPen;
	this->objectList = objectList;
	this->staticObjectsList = staticObjectsList;
	this->scene = scene;

	QGraphicsViewUserInput::initialize(scene);
	this->userInput = QGraphicsViewUserInput::getInstance();
}
