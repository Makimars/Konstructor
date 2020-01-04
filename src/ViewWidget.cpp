#include "ViewWidget.h"

ViewWidget::ViewWidget(QWidget *parent) : QGraphicsView (parent)
{
	this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	this->defaultPen = QPen(Qt::PenStyle::SolidLine);
	this->defaultBrush = QBrush(Qt::BrushStyle::TexturePattern);

	this->sketchScene = new QGraphicsScene(this);
	this->objectsInSketch = new QVector<DrawableObject*>;

	DrawablesFactory::initialise(&this->defaultBrush,
								 &this->defaultPen,
								 this->objectsInSketch,
								 this->sketchScene
								 );
	this->objectFactory = DrawablesFactory::getInstance();

	this->mousePoint = this->objectFactory->makePoint();
	//this->mousePoint->setHighlight(true);
	this->mousePoint->setHidden(true);
	this->mousePoint->setPen(&this->defaultPen);
	this->mousePoint->setBrush(&this->defaultBrush);
	this->sketchScene->addItem(mousePoint);

	this->sketchScene->setSceneRect(
				-Settings::defaultSketchWidth / 2,
				-Settings::defaultSketchHeight / 2,
				Settings::defaultSketchWidth,
				Settings::defaultSketchHeight);
	this->setScene(sketchScene);

	//tools initialisation
	this->selectedTool = nullptr;
	initialiseTools();

	repaint();
}

ViewWidget::~ViewWidget()
{

}

//----------	getters and setters    ----------

//----------	tools    ----------

void ViewWidget::setTool(QString toolName)
{
	if(this->selectedTool != nullptr)
		this->selectedTool->resetTool();
	switch(Global::toolNames.lastIndexOf(toolName))
	{
		case LINE_TOOL:
			this->selectedTool = LineTool::getInstance();
			break;
		case CIRCLE_TOOL:
			this->selectedTool = CircleTool::getInstance();
			break;
		case RECTANGLE_TOOL:
			this->selectedTool = RectangleTool::getInstance();
			break;
		case LABEL_TOOL:
			this->selectedTool = LabelTool::getInstance();
			break;
		case DIMENSION_TOOL:
			this->selectedTool = DimensionTool::getInstance();
			break;
		default:
			this->selectedTool = nullptr;
			break;
	}
}

void ViewWidget::resetTool()
{
	if(this->selectedTool != nullptr)
		this->selectedTool->resetTool();
}

//----------	file operations    ----------

void ViewWidget::loadFromFile(QString file)
{
    QStringList splited = file.trimmed().split(";");

	QVector<DrawableObject*> loadedObjects;

	foreach(QString line, splited)
	{
		QString typeName = line.section('{',0,0).trimmed();
		QString content = line.section('{',1,1).section('}',0,0);
		DrawableObject *createdObj;

		switch(Global::typeNames.lastIndexOf(typeName))
		{
			case 0:
				createdObj = new Point();
				break;
			case 1:
				createdObj = new Line();
				createdObj->loadRelations(&loadedObjects);
				break;
			case 2:
				createdObj = new Circle();
				break;
			default:
				createdObj = nullptr;
				break;
		}

		if(createdObj != nullptr)
		{
			createdObj->fromFileString(content);
			loadedObjects.append(createdObj);
		}
	}

	foreach(DrawableObject *obj, loadedObjects)
	{
		obj->loadRelations(&loadedObjects);
	}

	foreach(DrawableObject *obj, loadedObjects)
    {
		this->objectFactory->addDrawable(obj);
        this->objectsInSketch->append(obj);
    }
}

void ViewWidget::saveToFile(QString path)
{
	QFile targetFile(path);
	if(targetFile.open(QIODevice::WriteOnly))
	{
		for(int i = 0; i < this->objectsInSketch->length(); i++)
		{
			targetFile.write(
						this->objectsInSketch
						->at(i)
						->toFileString()
						.toLatin1() + "\n"
						);
		}
	}

}

void ViewWidget::initialiseTools()
{
	LineTool::initialise(this->mousePoint,
						 this->sketchScene,
						 &this->defaultBrush,
						 &this->defaultPen
						 );
	CircleTool::initialise(this->mousePoint,
						 this->sketchScene,
						 &this->defaultBrush,
						 &this->defaultPen
						 );
	LabelTool::initialise(this->mousePoint,
						 this->sketchScene,
						 &this->defaultBrush,
						 &this->defaultPen
						 );
	RectangleTool::initialise(this->mousePoint,
						 this->sketchScene,
						 &this->defaultBrush,
						 &this->defaultPen
						 );
	DimensionTool::initialise(this->mousePoint,
							  this->sketchScene,
							  &this->defaultBrush,
							  &this->defaultPen
							  );
}

//----------	tools processing    ----------

Point *ViewWidget::pointSnapping(Point *point){
	for(int i = 0; i < this->objectsInSketch->length(); i++)
	{
		DrawableObject *obj = this->objectsInSketch->at(i);
		if(obj->getType() == TYPE_POINT)
		{
			Point *p = dynamic_cast<Point*>(obj);
			if(point->distanceFrom(p->getLocation()) < Settings::pointSnappingDistance)
				return p;
		}
	}
	return nullptr;
}

Line *ViewWidget::lineSnapping(Point *point)
{
	for(int i = 0; i < this->objectsInSketch->length(); i++)
	{
		DrawableObject *obj = this->objectsInSketch->at(i);
		if(obj->getType() == TYPE_LINE)
		{
			Line *referenceLine = dynamic_cast<Line*>(obj);

			if(referenceLine->distanceFrom(point) < Settings::pointSnappingDistance)
				return referenceLine;
		}
	}
	return nullptr;
}

DrawableObject *ViewWidget::mouseSnapping()
{
	DrawableObject *snappedObject = pointSnapping(this->mousePoint);

	if(snappedObject == nullptr)
		snappedObject = lineSnapping(this->mousePoint);

	return snappedObject;
}

//----------	events    ----------

void ViewWidget::mouseClickedEvent(QMouseEvent *event)
{
	//TODO: reimplement click on drawable
	QGraphicsItem *item = this->sketchScene->itemAt(mousePoint->getLocation(), QTransform());
	if(item != nullptr)
		qDebug() << dynamic_cast<DrawableObject*>(item)->getType();
	//QGraphicsView::mousePressEvent(event);
}

void ViewWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
		this->grabbedPoint = pointSnapping(this->mousePoint);

	this->mouseMovedSincePressed = false;
}

void ViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	this->grabbedPoint = nullptr;

	if(event->button() == Qt::LeftButton)
	{
		if(this->selectedTool != nullptr)
			this->selectedTool->click(mouseSnapping(), this->mousePoint);
	}

	if(!this->mouseMovedSincePressed)
		mouseClickedEvent(event);
}


void ViewWidget::mouseMoveEvent(QMouseEvent *event)
{
	//update mouse position
	this->mousePoint->setLocation(mapToScene(event->pos()));
	this->mouseMovedSincePressed = true;

	Point *snapped = pointSnapping(this->mousePoint);
	if(snapped != nullptr & this->grabbedPoint != snapped)
		this->mousePoint->setLocation(snapped->getLocation());

	//draging plane
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	if(event->buttons() == Qt::RightButton)
	{
		this->translate(100,100);
		this->dragMode();

		this->dragStartX = event->x();
		this->dragStartY = event->y();
	}

	//draging point
	if(this->grabbedPoint != nullptr)
		grabbedPoint->setLocation(this->mousePoint->getLocation());

	sketchScene->update();
}

void ViewWidget::wheelEvent(QWheelEvent *event)
{
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);

	if	(	((event->delta() > 0) & !Settings::mouseWheelInvertedZoom)	||
			((event->delta() < 0) & Settings::mouseWheelInvertedZoom)	)
	{
		this->scale(Settings::mouseWheelZoomFactor,
					Settings::mouseWheelZoomFactor
					);
	}
	else
	{
		this->scale(1.0 / Settings::mouseWheelZoomFactor,
					1.0 / Settings::mouseWheelZoomFactor
					);
	}
}

void ViewWidget::keyPressEvent(QKeyEvent *event)
{
    emit keyPressed(event);
}
