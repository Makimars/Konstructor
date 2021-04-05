#include "PlaneFactory.h"

PlaneFactory *PlaneFactory::instance = nullptr;

void PlaneFactory::initialise(const Style *defaultStyle,
									QVector<DrawableObject*> *objectList,
									QVector<DrawableObject*> *staticObjectsList,
									QGraphicsScene *scene
									)
{
	if(PlaneFactory::instance == nullptr)
	{
		PlaneFactory::instance = new PlaneFactory(defaultStyle, objectList, staticObjectsList, scene);
	}
}

PlaneFactory *PlaneFactory::getInstance()
{
	return PlaneFactory::instance;
}

//----------     object creation     ---------

Point *PlaneFactory::makePoint()
{
	Point *point = new Point();
	point->setStyle(currentStyle);

	return point;
}

Point *PlaneFactory::makePoint(double x, double y)
{
	Point *point = makePoint();
	point->setLocation(x, y);

	return point;
}

Point *PlaneFactory::copyPoint(const Point *point)
{
	Point *newPoint = makePoint();
	newPoint->setLocation(point->getLocation());

	return newPoint;
}

Line *PlaneFactory::makeLine(Point *startPoint, Point *endPoint)
{
	Line *line = new Line(startPoint, endPoint);
	line->setStyle(currentStyle);

	return line;
}

Circle *PlaneFactory::makeCircle(Point *centerPoint)
{
	Circle *circle = new Circle(centerPoint);
	circle->setStyle(currentStyle);

	return circle;
}


Circle *PlaneFactory::makeCircle(Point *centerPoint, double radius)
{
	Circle *circle = makeCircle(centerPoint);
	circle->setRadius(radius);

	return circle;
}

Label *PlaneFactory::makeLabel(QPointF location)
{
	Label *label = new Label(location, "               ");
	label->setStyle(currentStyle);

	QObject::connect(label, &UserInputRequester::requestString,
					 this->userInput, &QGraphicsViewUserInput::requestString
					 );

	return label;
}

Arc *PlaneFactory::makeArc(Point *points[])
{
	Arc *arc = new Arc(points[0], points[1], points[2]);
	arc->setStyle(currentStyle);

	return arc;
}

PointDistanceConstraint *PlaneFactory::makePointDistanceConstraint(Point *originPoint, Point *drivenPoint)
{
	PointDistanceConstraint *constraint = new PointDistanceConstraint(originPoint, drivenPoint);
	constraint->setStyle(&constraintStyle);

	QObject::connect(constraint, &UserInputRequester::requestDouble,
					 this->userInput, &QGraphicsViewUserInput::requestDouble
					 );

	return constraint;
}

CircleRadiusConstraint *PlaneFactory::makeCircleRadiusConstraint(Circle *circle)
{
	CircleRadiusConstraint *constraint = new CircleRadiusConstraint(circle);
	constraint->setStyle(&constraintStyle);

	QObject::connect(constraint, &UserInputRequester::requestDouble,
					 this->userInput, &QGraphicsViewUserInput::requestDouble
					 );

	return constraint;
}

LineCenterPointConstraint *PlaneFactory::makeLineCenterPointConstraint(Line *line, Point *point)
{
	LineCenterPointConstraint *constraint = new LineCenterPointConstraint(line, point);
	constraint->setStyle(&constraintStyle);

	return constraint;
}

ParaelLinesConstraint *PlaneFactory::makeParaelLinesConstraint(Line *lineOne, Line *lineTwo)
{
	ParaelLinesConstraint *constraint = new ParaelLinesConstraint(lineOne, lineTwo);
	constraint->setStyle(&constraintStyle);

	return constraint;
}

LinesAngleConstraint *PlaneFactory::makeLinesAngleConstraint(Line *lineOne, Line *lineTwo)
{
	LinesAngleConstraint *constraint = new LinesAngleConstraint(lineOne, lineTwo);
	constraint->setStyle(&constraintStyle);

	QObject::connect(constraint, &UserInputRequester::requestDouble,
					 this->userInput, &QGraphicsViewUserInput::requestDouble
					 );

	return constraint;
}

void PlaneFactory::addToScene(DrawableObject *object)
{
	scene->addItem(object);
}

//----------     object managment     ---------

void PlaneFactory::addDrawable(DrawableObject *object)
{
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

void PlaneFactory::tryDeleteDrawable(DrawableObject *object)
{
	if(!this->objectList->contains(object) & !this->staticObjectsList->contains(object))
		deleteDrawable(object);
}

bool PlaneFactory::deleteDrawable(DrawableObject *object)
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

void PlaneFactory::deleteAll()
{
	foreach(DrawableObject *item, *this->objectList)
	{
		delete item;
	}
	objectList->clear();
	this->idCounter = 1;
}

void PlaneFactory::addStaticDrawable(DrawableObject *object)
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

void PlaneFactory::deleteAllStaticDrawables()
{
	foreach(DrawableObject *item, *this->staticObjectsList)
	{
		delete item;
	}
	staticObjectsList->clear();
	staticIdCounter = -1;
}

QVector<DrawableObject*> PlaneFactory::generateListFromSketch(QString sketch)
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
				createdObj->setStyle(currentStyle);
				break;
			case Global::Line:
				createdObj = new Line();
				createdObj->setStyle(currentStyle);
				break;
			case Global::Circle:
				createdObj = new Circle();
				createdObj->setStyle(currentStyle);
				break;
			case Global::Arc:
				createdObj = new Arc();
				createdObj->setStyle(currentStyle);
				break;
			case Global::Label:
				createdObj = new Label();
				createdObj->setStyle(currentStyle);
				break;
			case Global::CircleRadiusConstraint:
				createdObj = new CircleRadiusConstraint();
				createdObj->setStyle(&constraintStyle);
				break;
			case Global::CirclesRadiusDifferenceConstraint:
				createdObj = new CirclesRadiusDifferenceDimension();
				createdObj->setStyle(&constraintStyle);
				break;
			case Global::PointDistanceConstraint:
				createdObj = new PointDistanceConstraint();
				createdObj->setStyle(&constraintStyle);
				break;
			case Global::LineCenterPointConstraint:
				createdObj = new LineCenterPointConstraint();
				createdObj->setStyle(&constraintStyle);
				break;
			case Global::ParaelLinesConstraint:
				createdObj = new ParaelLinesConstraint();
				createdObj->setStyle(&constraintStyle);
				break;
			case Global::LinesAngleConstraint:
				createdObj = new LinesAngleConstraint();
				createdObj->setStyle(&constraintStyle);
				break;
			default:
				createdObj = nullptr;
				break;
		}

		if(UserInputRequester *constraint = dynamic_cast<UserInputRequester*>(createdObj))
		{
			QObject::connect(constraint, &UserInputRequester::requestDouble,
							 this->userInput, &QGraphicsViewUserInput::requestDouble
							 );
			QObject::connect(constraint, &UserInputRequester::requestString,
							 this->userInput, &QGraphicsViewUserInput::requestString
							 );
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

PlaneFactory::PlaneFactory(const Style *defaultStyle,
					QVector<DrawableObject*> *objectList,
					QVector<DrawableObject*> *staticObjectsList,
					QGraphicsScene *scene)
{
	this->currentStyle = defaultStyle;
	this->objectList = objectList;
	this->staticObjectsList = staticObjectsList;
	this->scene = scene;

	this->constraintStyle = *defaultStyle;
	this->constraintStyle.pen.setStyle(Qt::PenStyle::DotLine);

	QGraphicsViewUserInput::initialize(scene);
	this->userInput = QGraphicsViewUserInput::getInstance();
}
