#include "ViewWidget.h"

ViewWidget::ViewWidget(QWidget *parent) : QGraphicsView (parent)
{
	this->setAlignment(Qt::AlignCenter);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	this->setDragMode(DragMode::NoDrag);

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
	this->mousePoint->setHidden(true);
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
}

ViewWidget::~ViewWidget()
{

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

void ViewWidget::mousePressEvent(QMouseEvent *event)
{
	if(this->selectedTool == nullptr)
		QGraphicsView::mousePressEvent(event);

	if(event->button() == Qt::LeftButton)
		this->grabbedPoint = pointSnapping(this->mousePoint);

	this->prevX = event->x();
	this->prevY = event->y();
}

void ViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	viewport()->setCursor(Qt::CursorShape::ArrowCursor);

	if(this->selectedTool == nullptr)
		QGraphicsView::mouseReleaseEvent(event);

	this->grabbedPoint = nullptr;

	if(event->button() == Qt::LeftButton)
	{
		if(this->selectedTool != nullptr)
			this->selectedTool->click(mouseSnapping(), this->mousePoint);
	}
}


void ViewWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(this->selectedTool == nullptr)
		QGraphicsView::mouseMoveEvent(event);

	//update mouse position
	this->mousePoint->setLocation(mapToScene(event->pos()));

	Point *snapped = pointSnapping(this->mousePoint);
	if(snapped != nullptr & this->grabbedPoint != snapped)
		this->mousePoint->setLocation(snapped->getLocation());

	//draging plane
	if(event->buttons() == Qt::MouseButton::RightButton)
	{
		viewport()->setCursor(Qt::CursorShape::ClosedHandCursor);

		QScrollBar *hBar = horizontalScrollBar();
		QScrollBar *vBar = verticalScrollBar();
		hBar->setValue(hBar->value() + prevX - event->x());
		vBar->setValue(vBar->value() + prevY - event->y());
	}

	//draging point
	if(this->grabbedPoint != nullptr)
		grabbedPoint->setLocation(this->mousePoint->getLocation());

	this->sketchScene->update();
	this->prevX = event->x();
	this->prevY = event->y();
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
