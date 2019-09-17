#include "ViewWidget.h"

ViewWidget::ViewWidget(QWidget * parent) : QGraphicsView (parent)
{
    this->id_counter = 0;
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	this->sketch_scene = new QGraphicsScene(this);
	this->objects_in_sketch = new QVector<DrawableObject*>;
    this->sketch_scene->setSceneRect(-1000,-1000,2000,2000);

	this->setScene(sketch_scene);


	//test code
	Point * p1 = addPoint(-100,-100);
	Point * p2 = addPoint(-100, 100);
	Point * p3 = addPoint(100, 100);
	Point * p4 = addPoint(100,-100);
    addLine(p1, p2);
    addLine(p2, p3);
    addLine(p3, p4);
    addLine(p4, p1);


    addCircle(p3, 50);
	Point * p5 = addPoint(1000,1000);
	Point * p6 = addPoint(1200,1200);
    addLine(p5,p6);
    addLine(p4, p5);

	repaint();
}

ViewWidget::~ViewWidget()
{

}

//----------	getters and setters    ----------

void ViewWidget::setMainWindow(MainWindow * parent){
	this->main_window = parent;
}

//----------	tools    ----------

void ViewWidget::setTool(QString tool_name)
{
	this->selected_tool = tool_name;
}

//----------	file operations    ----------

void ViewWidget::loadFromFile(QString file)
{
    QStringList splited = file.trimmed().split(";");

    QVector<DrawableObject*> loaded_objects;
    QStringList type_names = {
        "Point",
        "Line",
        "Circle"
    };

    foreach(QString line, splited)
    {
        QString type_name = line.section('{',0,0).trimmed();
        QString content = line.section('{',1,1).section('}',0,0);
        DrawableObject * created_obj;

        switch(type_names.lastIndexOf(type_name))
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

    foreach(DrawableObject * obj, loaded_objects)
    {
        obj->loadRelations(&loaded_objects);
    }

    foreach(DrawableObject * obj, loaded_objects)
        addDrawable(obj);

}

void ViewWidget::saveToFile(QString path)
{
    QFile target_file(path);
    if(target_file.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < objects_in_sketch->length(); i++)
        {
            target_file.write(
                        objects_in_sketch
                            ->at(i)
                            ->toFileString()
                            .toLatin1() + "\n"
                        );
        }
    }

}

//----------	tools processing    ----------

Point * ViewWidget::pointSnapping(Point * point){
	//needs implementation
	return point;
}

//----------	object managment    ----------

DrawableObject * ViewWidget::addDrawable(DrawableObject * obj)
{
	if(objects_in_sketch->lastIndexOf(obj) != -1)
		return nullptr;

	obj->setId(this->id_counter);
    this->id_counter++;

	this->objects_in_sketch->append(obj);
    this->scene()->addItem(obj);

    return obj;
}

Point * ViewWidget::addPoint(double x, double y)
{
    Point * p = new Point(x,y);
	addDrawable(p);
	return p;
}

Line * ViewWidget::addLine(Point * start_point, Point * end_point)
{
	if(this->objects_in_sketch->lastIndexOf(start_point) == -1)
		this->addDrawable(start_point);
	if(this->objects_in_sketch->lastIndexOf(end_point) == -1)
		this->addDrawable(end_point);

    Line * ln = new Line(start_point, end_point);
	addDrawable(ln);
	return ln;
}

Circle * ViewWidget::addCircle(Point * center, double radius)
{
	if(this->objects_in_sketch->lastIndexOf(center) == -1)
		this->addDrawable(center);

	Circle * c = new Circle(center, radius);
    addDrawable(c);
	return c;
}

//----------	events    ----------

void ViewWidget::mouseClickedEvent(QMouseEvent * event)
{

}

void ViewWidget::mousePressEvent(QMouseEvent * event)
{

}

void ViewWidget::mouseReleaseEvent(QMouseEvent * event)
{

}


void ViewWidget::mouseMoveEvent(QMouseEvent * event){

    if(event->buttons() == Qt::RightButton)
    {
        this->translate(100,100);
        this->dragMode();

        this->drag_start_x = event->x();
        this->drag_start_y = event->y();
    }

}

void ViewWidget::wheelEvent(QWheelEvent * event)
{
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

void ViewWidget::keyPressEvent(QKeyEvent * event)
{

}
