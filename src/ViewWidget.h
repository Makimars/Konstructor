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
	void setTool(QString toolName);

	//file operations
	void loadFromFile(QString fileContents);
    void saveToFile(QString file);

    //axis
	Line *getAxisLine(int xDirection, int yDirection);

private:
	QGraphicsScene *sketchScene;

	//tools processing
	Point *mousePoint;
	Point *pointSnapping(Point *point);
	Line *lineSnapping(Point *point);
	DrawableObject *mouseSnapping();

	DrawablesFactory *objectFactory;

	//tools
	Tool *selectedTool;
	QPen defaultPen;
	QBrush defaultBrush;

    //object managment
	QVector<DrawableObject*> *objectsInSketch;

	Line *upUpAxis, *upDownAxis, *downDownAxis, *downUpAxis;

	//draging
	double dragStartX;
	double dragStartY;
	Point *grabbedPoint = nullptr;

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
