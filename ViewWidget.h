#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>

#include "mainwindow.h"
#include "ClassRepository/Drawables/Circle.h"
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
	Point *clicked_point, *previous_clicked_point, *mouse_point;
	Point *pointSnapping(Point *point);
	Line *lineSnapping(Point *point);

	//tools
	QString selected_tool;
    Line *line_preview;
    Circle *circle_preview;
    QVector<Line> rect_preview_lines;
    QVector<Point> rect_preview_points;


    //object managment
    QVector<DrawableObject*> *objects_in_sketch, *temporary_objects;

	Line *up_up_axis, *up_down_axis, *down_down_axis, *down_up_axis;

	void removeDrawable(DrawableObject *obj);
	DrawableObject *addDrawable(DrawableObject *obj);
	Point *addPoint(double x, double y);
	Point *addPoint(QPointF location);
	Line *addLine(Point *start_point, Point *end_point);
	Circle *addCircle(Point *center, double radius);

    unsigned int id_counter = 0;

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
	void escape();
};

#endif // DRAWINGWIDGET_H
