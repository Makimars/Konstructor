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

Label *Factory::makeLabel(QPointF location)
{
	Label *label = new Label(location, "               ");
	label->setStyle(currentStyle);

	QObject::connect(label, &UserInputRequester::requestString,
					 this->userInput, &QGraphicsViewUserInput::requestString
					 );

	return label;
}

Arc *Factory::makeArc(Point *points[])
{
	Arc *arc = new Arc(points[0], points[1], points[2]);
	arc->setStyle(currentStyle);

	return arc;
}

PointDistanceConstraint *Factory::makePointDistanceConstraint(Point *originPoint, Point *drivenPoint)
{
	PointDistanceConstraint *constraint = new PointDistanceConstraint(originPoint, drivenPoint);
	constraint->setStyle(currentStyle);

	QObject::connect(constraint, &UserInputRequester::requestDouble,
					 this->userInput, &QGraphicsViewUserInput::requestDouble
					 );

	return constraint;
}

CircleRadiusConstraint *Factory::makeCircleRadiusConstraint(Circle *circle)
{
	CircleRadiusConstraint *constraint = new CircleRadiusConstraint(circle);
	constraint->setStyle(currentStyle);

	QObject::connect(constraint, &UserInputRequester::requestDouble,
					 this->userInput, &QGraphicsViewUserInput::requestDouble
					 );

	return constraint;
}

LineCenterPointConstraint *Factory::makeLineCenterPointConstraint(Line *line, Point *point)
{
	LineCenterPointConstraint *constraint = new LineCenterPointConstraint(line, point);
	constraint->setStyle(currentStyle);

	return constraint;
}

ParaelLinesConstraint *Factory::makeParaelLinesConstraint(Line *lineOne, Line *lineTwo)
{
	ParaelLinesConstraint *constraint = new ParaelLinesConstraint(lineOne, lineTwo);
	constraint->setStyle(currentStyle);

	return constraint;
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

bool Factory::deleteDrawable(DrawableObject *object)
{
	if(object == nullptr)
		return false;
	if(object->hasGeometryUpdates())
		return false;

	if(objectList->contains(object))
		this->objectList->removeAll(object);
	if(this->scene->items().contains(object))
		this->scene->removeItem(object);

	delete object;
	object = nullptr;

	return true;
}

void Factory::deleteAll()
{
	foreach(DrawableObject *item, *this->objectList)
	{
		delete item;
	}
	objectList->clear();
	this->idCounter = 1;
}

void Factory::addStaticDrawable(DrawableObject *object)
{
	if(!this->objectList->contains(object) & !this->staticObjectsList->contains(object))
	{
		object->setStyle(currentStyle);
		object->setFlags(QGraphicsItem::ItemIsSelectable);
		object->setAcceptHoverEvents(true);

		this->staticObjectsList->append(object);
		this->scene->addItem(object);

		object->setId(staticIdCounter);
		staticIdCounter--;
	}
}

void Factory::deleteAllStaticDrawables()
{
	foreach(DrawableObject *item, *this->staticObjectsList)
	{
		delete item;
	}
	staticObjectsList->clear();
	staticIdCounter = -1;
}

QVector<DrawableObject*> Factory::generateListFromSketch(QString sketch)
{
	QStringList splited = sketch.trimmed().replace('\n',"").split(";");
	splited.removeAt(splited.length()-1);

	QVector<DrawableObject*> loadedObjects;

	foreach(QString line, splited)
	{
		QString type = line.section('{',0,0).trimmed();
		QString content = line.section('{',1,1).section('}',0,0);
		DrawableObject *createdObj;

		switch(QVariant::fromValue(type).toInt())
		{
			case Global::Point:
				createdObj = new Point();
				break;
			case Global::Line:
				createdObj = new Line();
				break;
			case Global::Circle:
				createdObj = new Circle();
				break;
			case Global::Arc:
				createdObj = new Arc();
				break;
			case Global::Label:
				createdObj = new Label();
				break;
			case Global::LineAngleDimension:
				createdObj = new LinesAngleDimension();
				break;
			case Global::CircleRadiusConstraint:
				createdObj = new CircleRadiusConstraint();
				break;
			case Global::CirclesRadiusDifferenceConstraint:
				createdObj = new CirclesRadiusDifferenceDimension();
				break;
			case Global::PointDistanceConstraint:
				createdObj = new PointDistanceConstraint();
				break;
			case Global::LineCenterPointConstraint:
				createdObj = new LineCenterPointConstraint();
				break;
			case Global::ParaelLinesConstraint:
				createdObj = new ParaelLinesConstraint();
				break;
			default:
				createdObj = nullptr;
				break;
		}

		if(createdObj != nullptr)
		{
			createdObj->loadVariables(content);
			loadedObjects.append(createdObj);
		}
	}

	foreach(DrawableObject *obj, loadedObjects)
	{
		obj->loadRelations(loadedObjects + *this->staticObjectsList);
	}

	return loadedObjects;
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
