#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QScrollBar>
#include <QMenu>

#include "MessagesManager.h"
#include "ClassRepository/Plane/Plane"

class ViewWidget : public QGraphicsView
{
	Q_OBJECT
public:
	ViewWidget(QWidget *parent = nullptr);
	~ViewWidget();

	//file operations
	void loadFromFile(QString fileContents);
    void saveToFile(QString file);

    //axis
	Line *getAxisLine(int xDirection, int yDirection);
private:
	QGraphicsScene *sketchScene;

	QMenu contextMenu;
	QAction constractionalToggle;
	QAction deleteObjectAction;

	Factory *objectFactory;

	//tools
	Tool *selectedTool;

	QVector<Style> styles;

    //object managment
	QVector<DrawableObject*> objectsInSketch;
	QVector<DrawableObject*> staticObjects;

	//draging
	int prevX, prevY;

	//initialisation
	void initializeTools();
	void initializeScene();

	/**
	 * @brief snapping to 100x100 grid
	 * @param mousePos scene coordinates
	 * @return snapped scene coordinates
	 */
	QPointF gridSnapping(QPointF mousePos);

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
	void mouseMoved(QPointF pos);

public slots:
	void setTool(int tool);
	void resetTool();
	void finishDrawing();

	void customContextMenuRequested(const QPoint &pos);

	void newSketchButtonClicked();
	void closeSketchButtonClicked();
};

#endif // DRAWINGWIDGET_H
