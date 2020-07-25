#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QScrollBar>

#include "MainWindow.h"
#include "MessagesManager.h"
#include "ClassRepository/Plane/Tools/DimensionTool.h"

class ViewWidget : public QGraphicsView
{
	Q_OBJECT
public:
	ViewWidget(QWidget *parent = nullptr);
	~ViewWidget();

	//file operations
	void newFile();
	void loadFromFile(QString fileContents);
    void saveToFile(QString file);

    //axis
	Line *getAxisLine(int xDirection, int yDirection);

	void requestDrawing();
	void requestDrawing(QString sketch);
private:
	QGraphicsScene *sketchScene;

	QMenu contextMenu;
	QAction constractionalToggle;
	QAction deleteObjectAction;

	//tools processing
	Point *mousePoint;

	Factory *objectFactory;

	//tools
	Plane::Tool *selectedTool;
	QPen defaultPen;
	QBrush defaultBrush;

    //object managment
	QVector<DrawableObject*> objectsInSketch;
	QVector<DrawableObject*> staticObjects;

	//draging
	int prevX, prevY;

	//initialisation
	void initializeTools();
	void initializeScene();

	//events
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);

signals:
    void keyPressed(QKeyEvent *event);
	void showStatusBarMessage(const QString &message, int timeout = 0);
	void returnDrawing(QVector<DrawableObject*> drawing);

public slots:
	void setTool(int tool);
	void resetTool();
	void finishDrawing();

	void customContextMenuRequested(const QPoint &pos);
};

#endif // DRAWINGWIDGET_H
