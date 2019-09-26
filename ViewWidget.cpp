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
	this->mouse_point->highlight = true;
	this->sketch_scene->addItem(mouse_point);

	this->previous_clicked_point = nullptr;
	this->clicked_point = nullptr;
    this->line_preview = nullptr;

	this->sketch_scene->setSceneRect(
				-Settings::default_sketch_width / 2,
				-Settings::default_sketch_height / 2,
				Settings::default_sketch_width,
				Settings::default_sketch_height);
	this->setScene(sketch_scene);

	//test code
    Point *p1 = addPoint(-100,-100);
    Point *p2 = addPoint(-100, 100);
    Point *p3 = addPoint(100, 100);
	Point *p4 = addPoint(100,-100);
	addLine(p1, p2);
	addLine(p2, p3);
	addLine(p3, p4);
	addLine(p4, p1);

	addCircle(p3, 50);

	repaint();
}

ViewWidget::~ViewWidget()
{

}

//----------	getters and setters    ----------

//----------	tools    ----------

void ViewWidget::setTool(QString tool_name)
{
	this->selected_tool = tool_name;
	if(!this->objects_in_sketch->contains(this->clicked_point))
		delete this->clicked_point;
	if(!this->objects_in_sketch->contains(this->previous_clicked_point))
		delete this->previous_clicked_point;
	this->clicked_point = nullptr;
	this->previous_clicked_point = nullptr;

	this->sketch_scene->removeItem(this->line_preview);
	//removeDrawable(this->line_preview);
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

void ViewWidget::removeDrawable(DrawableObject *obj)
{
	if(objects_in_sketch->contains(obj))
		this->objects_in_sketch->removeAll(obj);
	sketch_scene->removeItem(obj);
	if(obj != nullptr)
		delete obj;
	obj = nullptr;
}

DrawableObject *ViewWidget::addDrawable(DrawableObject *obj)
{
	if(this->objects_in_sketch->contains(obj))
		return nullptr;

	obj->setId(this->id_counter);
	this->id_counter++;

	this->sketch_scene->addItem(obj);
	this->objects_in_sketch->append(obj);

	return obj;
}

Point *ViewWidget::addPoint(double x, double y)
{
	Point *p = new Point(x,y);
	addDrawable(p);
	return p;
}

Point *ViewWidget::addPoint(QPointF location)
{
	return addPoint(location.x(), location.y());
}

Line *ViewWidget::addLine(Point *start_point, Point *end_point)
{
	if(this->objects_in_sketch->lastIndexOf(start_point) == -1)
		this->addDrawable(start_point);
	if(this->objects_in_sketch->lastIndexOf(end_point) == -1)
		this->addDrawable(end_point);

	Line *ln = new Line(start_point, end_point);
	addDrawable(ln);
	return ln;
}

Circle *ViewWidget::addCircle(Point *center, double radius)
{
	if(this->objects_in_sketch->lastIndexOf(center) == -1)
		this->addDrawable(center);

	Circle *c = new Circle(center, radius);
	addDrawable(c);
	return c;
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
	if(this->previous_clicked_point != nullptr &
		this->objects_in_sketch->lastIndexOf(this->previous_clicked_point) == -1)
	{
		delete this->previous_clicked_point;
	}
    this->previous_clicked_point = clicked_point;

    this->clicked_point = pointSnapping(this->mouse_point);
    if(clicked_point == mouse_point)
        clicked_point = new Point(mouse_point->getLocation());

    switch(Global::tool_names.lastIndexOf(this->selected_tool))
	{
		case 0:
			if(this->line_preview != nullptr)
				removeDrawable(this->line_preview);

			this->line_preview = new Line(clicked_point, mouse_point);
			sketch_scene->addItem(this->line_preview);

			if(this->previous_clicked_point != nullptr)
			{
				if(this->objects_in_sketch->lastIndexOf(previous_clicked_point) != -1)
					addDrawable(previous_clicked_point);
				addDrawable(clicked_point);
				addLine(previous_clicked_point, clicked_point);
			}
			break;
		case 1:

			break;
		case 2:

			break;
		default:

			break;
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

	if((event->delta() > 0) & !Settings::mouse_wheel_inverted_zoom)
	{
		this->scale(Settings::mouse_wheel_zoom_factor,
					Settings::mouse_wheel_zoom_factor
					);
	}
	else if ((event->delta() < 0) & Settings::mouse_wheel_inverted_zoom)
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
	switch (event->key()) {
		case Qt::Key::Key_Escape:
			emit escape();
			break;
	}

}
