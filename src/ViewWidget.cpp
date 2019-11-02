#include "ViewWidget.h"

ViewWidget::ViewWidget(QWidget *parent) : QGraphicsView (parent)
{
	this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);


	this->sketch_scene = new QGraphicsScene(this);
	this->objects_in_sketch = new QVector<DrawableObject*>;

	this->mouse_point = new Point();
	this->mouse_point->setHighlight(true);
	this->sketch_scene->addItem(mouse_point);

	this->sketch_scene->setSceneRect(
				-Settings::default_sketch_width / 2,
				-Settings::default_sketch_height / 2,
				Settings::default_sketch_width,
				Settings::default_sketch_height);
	this->setScene(sketch_scene);

	this->selected_tool = nullptr;

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
			this->selected_tool = LineTool::getInstance(this->mouse_point, this->sketch_scene);
			break;
		case CIRCLE_TOOL:
			this->selected_tool = CircleTool::getInstance(this->mouse_point, this->sketch_scene);
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

	if(selected_tool != nullptr)
	{
		connect(this->selected_tool, &Tool::addDrawable,
				this, &ViewWidget::addDrawable
				);
		connect(this->selected_tool, &Tool::tryDeleteDrawable,
				this, &ViewWidget::tryDeleteDrawable
				);
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
        addDrawable(obj);
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
		if(QString::compare(obj->getType(), QString("Point"), Qt::CaseInsensitive) == 0)
		{
			Point *p = (Point*)obj;
			if(point->distanceFrom(p->getLocation()) < Settings::point_snapping_distance)
				return p;
		}
	}
	return point;
}

Line *ViewWidget::lineSnapping(Point *point)
{
	for(int i = 0; i < this->objects_in_sketch->length(); i++)
	{
		DrawableObject *obj = this->objects_in_sketch->at(i);
		if(obj->getType() == "Line")
		{

		}
	}
	return nullptr;
}

//----------	object managment    ----------
void ViewWidget::deleteDrawable(DrawableObject *obj)
{
	removeDrawable(obj);

	if(obj != nullptr)
	{
		delete obj;
		obj = nullptr;
	}
}

void ViewWidget::removeDrawable(DrawableObject *obj)
{
	if(objects_in_sketch->contains(obj))
		this->objects_in_sketch->removeAll(obj);
	this->sketch_scene->removeItem(obj);
}

void ViewWidget::addDrawable(DrawableObject *obj)
{
	if(!this->objects_in_sketch->contains(obj))
	{
		this->objects_in_sketch->append(obj);
		this->sketch_scene->addItem(obj);

		obj->setId(this->id_counter);
		this->id_counter++;
	}
}

void ViewWidget::tryDeleteDrawable(DrawableObject *obj)
{
	if(!this->objects_in_sketch->contains(obj))
		delete obj;
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
		{
			bool existing = true;
			Point *snapped = pointSnapping(this->mouse_point);

			if(snapped == this->mouse_point)
			{
				snapped = this->mouse_point->Clone();
				existing = false;
			}

			this->selected_tool->click(snapped, existing);
		}
	}
}


void ViewWidget::mouseMoveEvent(QMouseEvent *event)
{
	//draging
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	if(event->buttons() == Qt::RightButton)
	{
		this->translate(100,100);
		this->dragMode();

		this->drag_start_x = event->x();
		this->drag_start_y = event->y();
	}

	//update
	sketch_scene->update();
	mouse_point->setLocation(mapToScene(event->pos()));

	Point *current_point = pointSnapping(mouse_point);
	if(current_point != mouse_point)
		mouse_point->setLocation(current_point->getLocation());

	//higlighting

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
