#include "ViewWidget.h"

ViewWidget::ViewWidget(QWidget *parent) : QGraphicsView (parent)
{

	this->setBackgroundBrush(QBrush(QColor::fromRgb(210,210,210)));

	this->setAlignment(Qt::AlignCenter);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	this->setDragMode(DragMode::NoDrag);

	this->defaultPen = QPen(Qt::PenStyle::SolidLine);
	this->defaultPen.setColor(Qt::black);
	this->defaultBrush = QBrush(Qt::BrushStyle::TexturePattern);
	this->defaultBrush.setColor(Qt::black);

	this->sketchScene = new QGraphicsScene(this);
	this->objectsInSketch = new QVector<DrawableObject*>;

	DrawablesFactory::initialise(&this->defaultBrush,
								 &this->defaultPen,
								 this->objectsInSketch,
								 this->sketchScene
								 );
	this->objectFactory = DrawablesFactory::getInstance();

	this->mousePoint = this->objectFactory->makePoint();

	initializeScene();

	//tools initialisation
	this->selectedTool = nullptr;
	initializeTools();
}

ViewWidget::~ViewWidget()
{
	QList<QGraphicsItem*> items = this->sketchScene->items();
	foreach(QGraphicsItem *item, items)
	{
		delete item;
	}
}

void ViewWidget::newFile()
{
	this->objectFactory->deleteAll();
}

//----------	file operations    ----------

void ViewWidget::loadFromFile(QString file)
{
	QStringList splited = file.trimmed().replace('\n',"").split(";");
	splited.removeAt(splited.length()-1);

	QVector<DrawableObject*> loadedObjects;

	foreach(QString line, splited)
	{
		QString type = line.section('{',0,0).trimmed();
		QString content = line.section('{',1,1).section('}',0,0);
		DrawableObject *createdObj;

		switch(QVariant::fromValue(type).toInt())
		{
			case Type_Point:
				createdObj = new Point();
				break;
			case Type_Line:
				createdObj = new Line();
				break;
			case Type_Circle:
				createdObj = new Circle();
				break;
			case Type_Label:
				createdObj = new Label();
				break;
			case Type_LineLengthDimension:
				createdObj = new LineLengthDimension();
				break;
			case Type_LineAngleDimension:
				createdObj = new LinesAngleDimension();
				break;
			case Type_CircleRadiusDimension:
				createdObj = new CircleRadiusDimension();
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
		obj->loadRelations(loadedObjects+this->staticObjects);
	}

	foreach(DrawableObject *obj, loadedObjects)
    {
		this->objectFactory->addDrawable(obj);
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

//----------    initialization    ----------

void ViewWidget::initializeTools()
{
	LineTool::initialize(this->mousePoint,
						this->sketchScene,
						&this->defaultBrush,
						&this->defaultPen
						 );
	CircleTool::initialize(this->mousePoint,
						this->sketchScene,
						&this->defaultBrush,
						&this->defaultPen
						 );
	LabelTool::initialize(this->mousePoint,
						this->sketchScene,
						&this->defaultBrush,
						&this->defaultPen
						 );
	RectangleTool::initialize(this->mousePoint,
							this->sketchScene,
							&this->defaultBrush,
							&this->defaultPen
							 );
	DimensionTool::initialize(this->mousePoint,
							this->sketchScene,
							&this->defaultBrush,
							&this->defaultPen
							  );
}

void ViewWidget::initializeScene()
{
	this->sketchScene->setSceneRect(
				-Settings::sketchSize-2,
				-Settings::sketchSize-2,
				2+Settings::sketchSize * 2,
				2+Settings::sketchSize * 2);

	//edges
	QPen edgesPen = QPen(Qt::black);
	edgesPen.setWidth(10);

	//edge lines
	QGraphicsLineItem *topLine = new QGraphicsLineItem(-Settings::sketchSize,-Settings::sketchSize,Settings::sketchSize,-Settings::sketchSize);
	QGraphicsLineItem *rightLine = new QGraphicsLineItem(-Settings::sketchSize,-Settings::sketchSize,-Settings::sketchSize,Settings::sketchSize);
	QGraphicsLineItem *leftLine = new QGraphicsLineItem(Settings::sketchSize,-Settings::sketchSize,Settings::sketchSize,Settings::sketchSize);
	QGraphicsLineItem *bottomLine = new QGraphicsLineItem(-Settings::sketchSize,Settings::sketchSize,Settings::sketchSize,Settings::sketchSize);
	topLine->setPen(edgesPen);
	rightLine->setPen(edgesPen);
	leftLine->setPen(edgesPen);
	bottomLine->setPen(edgesPen);
	this->sketchScene->addItem(topLine);
	this->sketchScene->addItem(rightLine);
	this->sketchScene->addItem(leftLine);
	this->sketchScene->addItem(bottomLine);

	//grid
	QPen gridPen = QPen(QColor::fromRgb(0,0,0,64));
	gridPen.setWidthF(0.2);
	const int gridSize = 100;
	double count = Settings::sketchSize / gridSize;

	//vertial grid lines
	for (int i = -count; i < count; i++)
	{
		QGraphicsLineItem *line = new QGraphicsLineItem(i*gridSize,-Settings::sketchSize,i*gridSize,Settings::sketchSize);
		line->setPen(gridPen);
		this->sketchScene->addItem(line);
	}
	//horizontal grid lines
	for (int i = -count; i < count; i++)
	{
		QGraphicsLineItem *line = new QGraphicsLineItem(-Settings::sketchSize, i*gridSize, Settings::sketchSize, i*gridSize);
		line->setPen(gridPen);
		this->sketchScene->addItem(line);
	}

	//axis
	QPen *axisPen = new QPen(Qt::black);
	axisPen->setWidth(3);

	Point *topPoint = this->objectFactory->makePoint(0, -Settings::sketchSize);
	Point *bottomPoint = this->objectFactory->makePoint(0, Settings::sketchSize);
	Point *leftPoint = this->objectFactory->makePoint(-Settings::sketchSize, 0);
	Point *rightPoint = this->objectFactory->makePoint(Settings::sketchSize, 0);

	Line *yAxis = this->objectFactory->makeLine(bottomPoint, topPoint);
	yAxis->setPen(axisPen);
	yAxis->setId(Y_AXIS_ID);
	yAxis->setAcceptHoverEvents(true);
	this->sketchScene->addItem(yAxis);
	this->staticObjects.append(yAxis);

	Line *xAxis = this->objectFactory->makeLine(leftPoint, rightPoint);
	xAxis->setPen(axisPen);
	xAxis->setId(X_AXIS_ID);
	xAxis->setAcceptHoverEvents(true);
	this->sketchScene->addItem(xAxis);
	this->staticObjects.append(xAxis);

	Point *zeroPoint = this->objectFactory->makePoint(0,0);
	zeroPoint->setId(ZERO_POINT_ID);
	zeroPoint->setAcceptHoverEvents(true);
	this->sketchScene->addItem(zeroPoint);
	this->staticObjects.append(zeroPoint);

	this->setScene(sketchScene);
}

//----------	events    ----------

void ViewWidget::mousePressEvent(QMouseEvent *event)
{
	QGraphicsView::mousePressEvent(event);

	QGraphicsViewUserInput *userInput = QGraphicsViewUserInput::getInstance();
	userInput->setInputBoxLocation(this->mousePoint->getLocation());
	if(!userInput->isFocused())
		userInput->closeInputBox();

	this->prevX = event->x();
	this->prevY = event->y();
}

void ViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	viewport()->setCursor(Qt::CursorShape::ArrowCursor);
	QGraphicsView::mouseReleaseEvent(event);

	if(event->button() == Qt::LeftButton)
	{
		if(this->selectedTool != nullptr)
			this->selectedTool->click(dynamic_cast<DrawableObject*>(this->itemAt(event->pos())), this->mousePoint);
	}
}

void ViewWidget::mouseMoveEvent(QMouseEvent *event)
{
	QGraphicsView::mouseMoveEvent(event);

	//update mouse position
	this->mousePoint->setLocation(mapToScene(event->pos()));

	DrawableObject *snapped = dynamic_cast<DrawableObject*>(this->itemAt(event->pos()));
	if(snapped != nullptr)
		if(snapped->getType() == Type_Point)
			this->mousePoint->setLocation(dynamic_cast<Point*>(snapped)->getLocation());

	//draging plane
	if(event->buttons() == Qt::MouseButton::RightButton)
	{
		viewport()->setCursor(Qt::CursorShape::ClosedHandCursor);

		QScrollBar *hBar = horizontalScrollBar();
		QScrollBar *vBar = verticalScrollBar();
		hBar->setValue(hBar->value() + prevX - event->x());
		vBar->setValue(vBar->value() + prevY - event->y());
	}

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
	QGraphicsView::keyPressEvent(event);
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
