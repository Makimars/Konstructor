#include "PlaneWidget.h"

PlaneWidget::PlaneWidget(QWidget *parent) : QGraphicsView (parent)
{

	this->setBackgroundBrush(QBrush(QColor::fromRgb(210,210,210)));
	this->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
	connect(this, &QGraphicsView::customContextMenuRequested,
			this, &PlaneWidget::customContextMenuRequested
			);

	//context menu
	constractionalToggle.setCheckable(true);
	constractionalToggle.setText(tr("Constructional"));
	contextMenu.addAction(&constractionalToggle);
	deleteObjectAction.setText(tr("Delete"));
	contextMenu.addAction(&deleteObjectAction);
	lockPointAction.setCheckable(true);
	lockPointAction.setText(tr("Lock point"));
	contextMenu.addAction(&lockPointAction);
	lockPointAction.setVisible(false);

	this->setAlignment(Qt::AlignCenter);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	this->setDragMode(DragMode::NoDrag);

	//Style
	QPen pen(Qt::PenStyle::SolidLine);
	pen.setColor(Qt::black);
	QBrush brush(Qt::black, Qt::BrushStyle::NoBrush);
	styles.append(Style(pen, brush));

	this->sketchScene = new QGraphicsScene(this);

	PlaneFactory::initialise(&styles.at(0),
								 &this->objectsInSketch,
								 &this->staticObjects,
								 this->sketchScene
								 );
	this->planeFactory = PlaneFactory::getInstance();

	initializeScene();

	//tools initialisation
	this->selectedTool = nullptr;
	initializeTools();
}

//----------	file operations    ----------

void PlaneWidget::loadFromFile(QString file)
{
	QVector<DrawableObject*> loadedObjects = planeFactory->generateListFromSketch(file);

	foreach(DrawableObject *obj, loadedObjects)
    {
		this->planeFactory->addDrawable(obj);
    }
}

QString PlaneWidget::toFile()
{
	nlohmann::json json;

	for(int i = 0; i < this->objectsInSketch.length(); i++)
		json.push_back(this->objectsInSketch.at(i)->toJson());

	return QString::fromStdString(std::string(json.dump()));
}

void PlaneWidget::loadProjected(QPolygonF projectedPoints)
{
	planeFactory->deleteAllStaticDrawables();
	if(projectedPoints.size() < 1) return;

	std::vector<Point*> points;
	std::vector<Line*> lines;

	for (int i = 0; i < projectedPoints.size(); i++ )
		points.push_back(planeFactory->makePoint(projectedPoints.at(i).x(), projectedPoints.at(i).y()));

	for (int i = 0; i < projectedPoints.size()-1; i++ )
		lines.push_back(planeFactory->makeLine(points.at(i), points.at(i+1)));
	lines.push_back(planeFactory->makeLine(points.at(points.size()-1), points.at(0)));


	for(uint32_t i = 0; i < points.size(); i++)
		planeFactory->addStaticDrawable(points.at(i));
	for(uint32_t i = 0; i < lines.size(); i++)
		planeFactory->addStaticDrawable(lines.at(i));
}

//----------    initialization    ----------

void PlaneWidget::initializeTools()
{
	connect(this, &PlaneWidget::mouseMoved,
			ArcTool::getInstance(), &Tool::mouseMoveEvent
			);
	connect(this, &PlaneWidget::mouseMoved,
			CircleTool::getInstance(), &Tool::mouseMoveEvent
			);
	connect(this, &PlaneWidget::mouseMoved,
			LabelTool::getInstance(), &Tool::mouseMoveEvent
			);
	connect(this, &PlaneWidget::mouseMoved,
			LineTool::getInstance(), &Tool::mouseMoveEvent
			);
	connect(this, &PlaneWidget::mouseMoved,
			RectangleTool::getInstance(), &Tool::mouseMoveEvent
			);
	connect(this, &PlaneWidget::mouseMoved,
			ExpandPolygonTool::getInstance(), &Tool::mouseMoveEvent
			);
}

void PlaneWidget::initializeScene()
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
	QPen zeroPen = gridPen;
	zeroPen.setWidth(2);

	const int gridSize = 100;
	double count = Settings::sketchSize / gridSize;

	//vertial grid lines
	for (int i = -count; i < count; i++)
	{
		QGraphicsLineItem *line = new QGraphicsLineItem(i*gridSize,-Settings::sketchSize,i*gridSize,Settings::sketchSize);

		if(i == 0) line->setPen(zeroPen);
		else line->setPen(gridPen);

		this->sketchScene->addItem(line);
	}
	//horizontal grid lines
	for (int i = -count; i < count; i++)
	{
		QGraphicsLineItem *line = new QGraphicsLineItem(-Settings::sketchSize, i*gridSize, Settings::sketchSize, i*gridSize);

		if(i == 0) line->setPen(zeroPen);
		else line->setPen(gridPen);

		this->sketchScene->addItem(line);
	}

	mousePosLabel.setStyleSheet("background-color: rgba(255, 255, 255, 0);");
	mousePosLabel.setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding));
	mousePosLabelProxy = sketchScene->addWidget(&mousePosLabel);

	this->setScene(sketchScene);
}

QPointF PlaneWidget::gridSnapping(QPointF mousePos)
{
	if(Settings::gridSnapping)
	{
		static int threshold = 10;
		double x = (int)mousePos.x() % 100;
		double y = (int)mousePos.y() % 100;

		//close to 0;0
		if(std::sqrt(x*x + y*y) < threshold)
		{
			mousePos.setX((int)mousePos.x() - x);
			mousePos.setY((int)mousePos.y() - y);
		}
		//close to 100;100
		else if(std::sqrt((100-x)*(100-x) + (100-y)*(100-y)) < threshold)
		{
			mousePos.setX((int)mousePos.x() + (100-x));
			mousePos.setY((int)mousePos.y() + (100-y));
		}
		//close to 0;100
		else if(std::sqrt(x*x + (100-y)*(100-y)) < threshold)
		{
			mousePos.setX((int)mousePos.x() + x);
			mousePos.setY((int)mousePos.y() + (100-y));
		}
		//close to 100;0
		else if(std::sqrt((100-x)*(100-x) + y*y) < threshold)
		{
			mousePos.setX((int)mousePos.x() + (100-x));
			mousePos.setY((int)mousePos.y() + y);
		}
	}

	return mousePos;
}

//----------	events    ----------

void PlaneWidget::mousePressEvent(QMouseEvent *event)
{
	QGraphicsView::mousePressEvent(event);

	QGraphicsViewUserInput *userInput = QGraphicsViewUserInput::getInstance();
	userInput->setInputBoxLocation(mapToScene(event->pos()));

	if(!userInput->isFocused())
		userInput->closeInputBox();

	this->prevX = event->x();
	this->prevY = event->y();
}

void PlaneWidget::mouseReleaseEvent(QMouseEvent *event)
{
	viewport()->setCursor(Qt::CursorShape::ArrowCursor);
	QGraphicsView::mouseReleaseEvent(event);

	if(event->button() == Qt::LeftButton)
	{
		if(this->selectedTool != nullptr)
		{
			DrawableObject *clickedObject = dynamic_cast<DrawableObject*>(this->itemAt(event->pos()));
			if(clickedObject == nullptr || !(objectsInSketch.contains(clickedObject) || staticObjects.contains(clickedObject)))
					clickedObject = nullptr;

			this->selectedTool->click(clickedObject, gridSnapping(mapToScene(event->pos())));

			emit showStatusBarMessage(selectedTool->getToolTip());
		}
	}
}

void PlaneWidget::mouseMoveEvent(QMouseEvent *event)
{
	//draging plane
	if(event->buttons() == Qt::MouseButton::MidButton)
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


	QPointF mousePoint = gridSnapping(mapToScene(event->pos()));

	event->setLocalPos(mapFromScene(mousePoint));
	QGraphicsView::mouseMoveEvent(event);

	emit mouseMoved(mousePoint);

	mousePosLabelProxy->setPos(mousePoint.x(), mousePoint.y()-20);
	mousePosLabel.setText("X: " + QString::number((int)mousePoint.x()) + "; Y: " + QString::number((int)mousePoint.y()));
	mousePosLabel.adjustSize();
}

void PlaneWidget::wheelEvent(QWheelEvent *event)
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

void PlaneWidget::keyPressEvent(QKeyEvent *event)
{
	QGraphicsView::keyPressEvent(event);
	emit keyPressed(event);
}

//----------	tools    ----------

void PlaneWidget::setTool(int tool)
{
	if(this->selectedTool != nullptr)
		this->selectedTool->resetTool();
	switch(tool)
	{
		case Global::Tools::LineTool:
			this->selectedTool = LineTool::getInstance();
			break;
		case Global::Tools::CircleTool:
			this->selectedTool = CircleTool::getInstance();
			break;
		case Global::Tools::RectangleTool:
			this->selectedTool = RectangleTool::getInstance();
			break;
		case Global::Tools::LabelTool:
			this->selectedTool = LabelTool::getInstance();
			break;
		case Global::Tools::ArcTool:
			this->selectedTool = ArcTool::getInstance();
			break;
		case Global::Tools::PointPositionTool:
			this->selectedTool = PointPositionTool::getInstance();
			break;
		case Global::Tools::LockPointTool:
			this->selectedTool = LockPositionTool::getInstance();
			break;
		case Global::Tools::PointDistanceConstrainTool:
			this->selectedTool = PointDistanceConstrainTool::getInstance();
			break;
		case Global::Tools::CircleRadiusConstraintTool:
			this->selectedTool = CircleRadiusConstraintTool::getInstance();
			break;
		case Global::Tools::ExpandPolgyonTool:
			this->selectedTool = ExpandPolygonTool::getInstance();
			break;
		case Global::Tools::LineCenterTool:
			this->selectedTool = LineCenterTool::getInstance();
			break;
		case Global::Tools::ParaelLinesConstraintTool:
			this->selectedTool = ParaelLinesConstraintTool::getInstance();
			break;
		case Global::Tools::LinesAngleConstraintTool:
			this->selectedTool = LinesAngleConstraintTool::getInstance();
			break;
		default:
			this->selectedTool = nullptr;
			emit showStatusBarMessage("");
			break;
	}

	if(this->selectedTool != nullptr)
		emit showStatusBarMessage(selectedTool->getToolTip());
}

void PlaneWidget::resetTool()
{
	if(this->selectedTool != nullptr)
	{
		this->selectedTool->resetTool();
		emit showStatusBarMessage(selectedTool->getToolTip());
	}
}

QVector<DrawableObject*> PlaneWidget::finishDrawing()
{
	return this->objectsInSketch + staticObjects;
}

void PlaneWidget::customContextMenuRequested(const QPoint &pos)
{
	if(this->selectedTool != nullptr)
	{
		selectedTool->resetTool();
	}

	if(DrawableObject *obj = dynamic_cast<DrawableObject*>(this->itemAt(pos)))
	{
		if(obj->getId() <= 0) return;

		obj->setHighlight(true);
		constractionalToggle.setChecked(obj->isConstructional());

		if(Point *point = dynamic_cast<Point*>(obj))
		{
			lockPointAction.setChecked(point->isLocked());
			lockPointAction.setVisible(true);
		}

		QAction *selectedAction = contextMenu.exec(this->viewport()->mapToGlobal(pos));
		if((selectedAction == &deleteObjectAction) & !(obj->isLocked()))
		{
			obj->removeGeometryUpdates();
			if(!planeFactory->deleteDrawable(obj))
			{
				emit showStatusBarMessage(("Cannot delete object, others depends on it."));
			}
		}

		obj->setLocked(lockPointAction.isChecked());
		obj->setIsConstructional(constractionalToggle.isChecked());
		obj->setHighlight(false);

		lockPointAction.setVisible(false);
	}
}

void PlaneWidget::newSketchButtonClicked()
{
	this->planeFactory->deleteAll();
}

void PlaneWidget::closeSketchButtonClicked()
{
	planeFactory->deleteAll();
	planeFactory->deleteAllStaticDrawables();
	emit closeDrawing();
}
