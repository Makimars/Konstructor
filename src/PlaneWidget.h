#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QScrollBar>
#include <QMenu>
#include <QToolTip>

#include "Dialogs/MessagesManager.h"
#include "Plane/Plane"

class PlaneWidget : public QGraphicsView
{
	Q_OBJECT
public:
	PlaneWidget(QWidget *parent = nullptr);

	//file operations
	void loadFromFile(QString fileContents);
	QString toFile();

	void loadProjected(QPolygonF projectedPoints);
	QVector<DrawableObject*> finishDrawing();

    //axis
	Line *getAxisLine(int xDirection, int yDirection);
private:
	QGraphicsScene *sketchScene;

	QMenu contextMenu;
	QAction constractionalToggle;
	QAction deleteObjectAction;
	QAction lockPointAction;

	QLabel mousePosLabel;
	QGraphicsProxyWidget *mousePosLabelProxy;

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
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

signals:
    void keyPressed(QKeyEvent *event);
	void showStatusBarMessage(const QString &message, int timeout = 0);
	void mouseMoved(QPointF pos);
	void closeDrawing();

public slots:
	void setTool(int tool);
	void resetTool();

	void customContextMenuRequested(const QPoint &pos);

	void newSketchButtonClicked();
	void closeSketchButtonClicked();
};

#endif // DRAWINGWIDGET_H
