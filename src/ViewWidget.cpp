#include "ViewWidget.h"

ViewWidget::ViewWidget(QWidget *parent) : QGraphicsView (parent)
{
	this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	this->default_pen = QPen(Qt::PenStyle::SolidLine);
	this->default_brush = QBrush(Qt::BrushStyle::TexturePattern);

	this->sketch_scene = new QGraphicsScene(this);
	this->objects_in_sketch = new QVector<DrawableObject*>;

	DrawablesFactory::initialise(&this->default_brush,
								 &this->default_pen,
								 this->objects_in_sketch,
								 this->sketch_scene
								 );
	this->object_factory = DrawablesFactory::getInstance();

	this->mouse_point = this->object_factory->makePoint();
	this->mouse_point->setHighlight(true);
	this->mouse_point->setPen(&this->default_pen);
	this->mouse_point->setBrush(&this->default_brush);
	this->sketch_scene->addItem(mouse_point);

	this->sketch_scene->setSceneRect(
				-Settings::default_sketch_width / 2,
				-Settings::default_sketch_height / 2,
				Settings::default_sketch_width,
				Settings::default_sketch_height);
	this->setScene(sketch_scene);

	//tools initialisation
	this->selected_tool = nullptr;
	LineTool::initialise(this->mouse_point,
						 this->sketch_scene,
						 &this->default_brush,
						 &this->default_pen
						 );
	CircleTool::initialise(this->mouse_point,
						 this->sketch_scene,
						 &this->default_brush,
						 &this->default_pen
						 );
	LabelTool::initialise(this->mouse_point,
						 this->sketch_scene,
						 &this->default_brush,
						 &this->default_pen
						 );
	RectangleTool::initialise(this->mouse_point,
						 this->sketch_scene,
						 &this->default_brush,
						 &this->default_pen
						 );

	repaint();
}

ViewWidget::~ViewWidget()
{

}

//----------	getters and setters    ----------

//----------	tools    ----------

void ViewWidget::setTool(QString tool_name)
{
	if(this->selected_tool != nullptr)
		this->selected_tool->resetTool();

	switch(Global::tool_names.lastIndexOf(tool_name))
	{
		case LINE_TOOL:
			this->selected_tool = LineTool::getInstance();
			break;
		case CIRCLE_TOOL:
			this->selected_tool = CircleTool::getInstance();
			break;
		case RECTANGLE_TOOL:
			this->selected_tool = RectangleTool::getInstance();
			break;
		case LABEL_TOOL:
			this->selected_tool = LabelTool::getInstance();
			break;
		default:
			this->selected_tool = nullptr;
			break;
	}
}

//----------	file operations    ----------

void ViewWidget::loadFromFile(QString file)
{
    QStringList splited = file.trimmed().split(";");

    QVector<DrawableObject*> loaded_objects;

	foreach(QString line, splited)
	{
		QString type_name = line.section('{',0,0).trimmed();
		QString content = line.section('{',1,1).section('}',0,0);
        DrawableObject *created_obj;

        switch(Global::type_names.lastIndexOf(type_name))
		{
			case 0:
				created_obj = new Point();
				break;
			case 1:
				created_obj = new Line();
				created_obj->loadRelations(&loaded_objects);
				break;
			case 2:
				created_obj = new Circle();
				break;
			default:
				created_obj = nullptr;
				break;
		}

		if(created_obj != nullptr)
		{
			created_obj->fromFileString(content);
			loaded_objects.append(created_obj);
		}
	}

	foreach(DrawableObject *obj, loaded_objects)
	{
		obj->loadRelations(&loaded_objects);
	}

	foreach(DrawableObject *obj, loaded_objects)
    {
		this->object_factory->addDrawable(obj);
        this->objects_in_sketch->append(obj);
    }
}

void ViewWidget::saveToFile(QString path)
{
	QFile target_file(path);
	if(target_file.open(QIODevice::WriteOnly))
	{
		for(int i = 0; i < this->objects_in_sketch->length(); i++)
		{
			target_file.write(
						this->objects_in_sketch
						->at(i)
						->toFileString()
						.toLatin1() + "\n"
						);
		}
	}

}

//----------	tools processing    ----------

Point *ViewWidget::pointSnapping(Point *point){
	for(int i = 0; i < this->objects_in_sketch->length(); i++)
	{
		DrawableObject *obj = this->objects_in_sketch->at(i);
		if(obj->getType() == TYPE_POINT)
		{
			Point *p = dynamic_cast<Point*>(obj);
			if(point->distanceFrom(p->getLocation()) < Settings::point_snapping_distance)
				return p;
		}
	}
	return nullptr;
}

Line *ViewWidget::lineSnapping(Point *point)
{
	for(int i = 0; i < this->objects_in_sketch->length(); i++)
	{
		DrawableObject *obj = this->objects_in_sketch->at(i);
		if(obj->getType() == TYPE_LINE)
		{
			Line *reference_line = dynamic_cast<Line*>(obj);

			if(reference_line->distanceFrom(point) > Settings::point_snapping_distance)
				return reference_line;
		}
	}
	return nullptr;
}

DrawableObject *ViewWidget::mouseSnapping()
{
	DrawableObject *snapped_object = pointSnapping(this->mouse_point);

	if(snapped_object == nullptr)
		snapped_object = lineSnapping(this->mouse_point);

	return snapped_object;
}

//----------	events    ----------

void ViewWidget::mouseClickedEvent(QMouseEvent *event)
{

}

void ViewWidget::mousePressEvent(QMouseEvent *event)
{

}

void ViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		if(this->selected_tool != nullptr)
			this->selected_tool->click(mouseSnapping(), this->mouse_point);
	}
}


void ViewWidget::mouseMoveEvent(QMouseEvent *event)
{
	//update mouse position
	mouse_point->setLocation(mapToScene(event->pos()));

	//draging plane
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	if(event->buttons() == Qt::RightButton)
	{
		this->translate(100,100);
		this->dragMode();

		this->drag_start_x = event->x();
		this->drag_start_y = event->y();
	}

	//draging points
	Point *snapped_point = pointSnapping(this->mouse_point);

	if(snapped_point != nullptr)
	{
		if(event->buttons() == Qt::LeftButton)
			snapped_point->setLocation(this->mouse_point->getLocation());
		else
			mouse_point->setLocation(snapped_point->getLocation());
	}



	//higlighting


	sketch_scene->update();
}

void ViewWidget::wheelEvent(QWheelEvent *event)
{
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);

	if	(	((event->delta() > 0) & !Settings::mouse_wheel_inverted_zoom)	||
			((event->delta() < 0) & Settings::mouse_wheel_inverted_zoom)	)
	{
		this->scale(Settings::mouse_wheel_zoom_factor,
					Settings::mouse_wheel_zoom_factor
					);
	}
	else
	{
		this->scale(1.0 / Settings::mouse_wheel_zoom_factor,
					1.0 / Settings::mouse_wheel_zoom_factor
					);
	}
}

void ViewWidget::keyPressEvent(QKeyEvent *event)
{
    emit keyPressed(event);
}
