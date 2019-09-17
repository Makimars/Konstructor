#include "ViewWidget.h"

ViewWidget::ViewWidget(QWidget * parent) : QGraphicsView (parent)
{
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    this->sketch_scene = new QGraphicsScene(this);
    this->sketch_plane = new Plane();
    this->mouse_point = new Point();

    this->sketch_scene->setSceneRect(-1000,-1000,2000,2000);
	this->setScene(sketch_scene);

    this->sketch_scene->addItem(mouse_point);

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
        this->sketch_plane->addDrawable(obj);

    foreach(DrawableObject * obj, loaded_objects)
        this->sketch_scene->addItem(obj);

}

void ViewWidget::saveToFile(QString path)
{
    QFile target_file(path);
    if(target_file.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < this->sketch_plane->objects->length(); i++)
        {
            target_file.write(
                        this->sketch_plane->objects
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


void ViewWidget::mouseMoveEvent(QMouseEvent * event)
{
    //draging
    if(event->buttons() == Qt::RightButton)
    {
        this->translate(100,100);
        this->dragMode();

        this->drag_start_x = event->x();
        this->drag_start_y = event->y();
    }

    mouse_point->setLocation(mapToScene(event->pos()));
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
