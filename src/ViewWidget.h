#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>

#include "mainwindow.h"
#include "ClassRepository/Tools/DimensionTool.h"
#include "ClassRepository/GlobalVariables.h"

class ViewWidget : public QGraphicsView
{
	Q_OBJECT
public:
	ViewWidget(QWidget *parent = nullptr);
	~ViewWidget();

	//getters and setters

	//tools
	void setTool(QString tool_name);

	//file operations
	void loadFromFile(QString file_contents);
    void saveToFile(QString file);

    //axis
	Line *getAxisLine(int x_direction, int y_direction);

private:
	QGraphicsScene *sketch_scene;

	//tools processing
	Point *mouse_point;
	Point *pointSnapping(Point *point);
	Line *lineSnapping(Point *point);
	DrawableObject *mouseSnapping();

	DrawablesFactory *object_factory;

	//tools
	Tool *selected_tool;
	QPen default_pen;
	QBrush default_brush;

    //object managment
	QVector<DrawableObject*> *objects_in_sketch;

	Line *up_up_axis, *up_down_axis, *down_down_axis, *down_up_axis;

	//draging
	double drag_start_x;
	double drag_start_y;

	//events
	void mouseClickedEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);

signals:
    void keyPressed(QKeyEvent *event);

};

#endif // DRAWINGWIDGET_H
