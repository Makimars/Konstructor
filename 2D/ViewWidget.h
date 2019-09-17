#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>
#include "mainwindow.h"
#include "ClassRepository/Drawables/Circle.h"

class ViewWidget : public QGraphicsView
{
	Q_OBJECT
public:
	ViewWidget(QWidget * parent = nullptr);
	~ViewWidget();

	//getters and setters
	void setMainWindow(MainWindow * parent);

	//tools
	void setTool(QString tool_name);

	//file operations
	void loadFromFile(QString file_contents);
    void saveToFile(QString file);

private:
	MainWindow * main_window;
    QGraphicsScene * sketch_scene;

	QVector<DrawableObject*> * objects_in_sketch;
    unsigned int id_counter = 0;


	//tools processing
    Point * last_click_point, * second_last_click_point;
	Point *pointSnapping(Point *point);

	//tools
	QString selected_tool;

	//object managment
    DrawableObject * addDrawable(DrawableObject * obj);
	Point * addPoint(double x, double y);
	Line * addLine(Point * start_point, Point * end_point);
	Circle * addCircle(Point * center, double radius);

	//events
	void mouseClickedEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);
	void wheelEvent(QWheelEvent * event);
	void keyPressEvent(QKeyEvent * event);

    //draging
    double drag_start_x;
    double drag_start_y;

};

#endif // DRAWINGWIDGET_H
