#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>

#include "mainwindow.h"
#include "ClassRepository/Tools/LabelTool.h"
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

	//tools
	Tool *selected_tool;

    //object managment
    QVector<DrawableObject*> *objects_in_sketch, *temporary_objects;

	Line *up_up_axis, *up_down_axis, *down_down_axis, *down_up_axis;

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
    void keyPressed(QKeyEvent *event);

private slots:
	/**
	 * @brief removes drawable from object list and view, and deletes it
	 * @param obj
	 */
	void deleteDrawable(DrawableObject *obj);

	/**
	 * @brief removes the drawable from object list and the scene
	 * @param obj
	 */
	void removeDrawable(DrawableObject *obj);
	/**
	 * @brief adds drawable to scene and object list
	 * @param obj
	 * @return
	 */
	void addDrawable(DrawableObject *obj);
	/**
	 * @brief deletes drawable if it is not in object list
	 * @param obj
	 */
	void tryDeleteDrawable(DrawableObject *obj);

};

#endif // DRAWINGWIDGET_H
