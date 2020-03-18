#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H


#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QtMath>
#include <QVector2D>

#include <QDebug>

#include "src/ClassRepository/Settings.h"
#include "src/ClassRepository/GlobalVariables.h"

class DrawableObject : public QGraphicsItem
{
public:
	virtual void resolveTies() {}

	//file handling

	/**
	 * @brief asigns variables from a file representation
	 * @param input
	 */
	virtual void loadVariables(QString input){}

	/**
	 * @brief generates file representation of an object
	 * @return file
	 */
    virtual QString toFileString();

	/**
	 * @brief assign pointers to objects to a variable from a vector of drawables
	 * @param list
	 */
	virtual void loadRelations(QVector<DrawableObject*> list) {}

	//getters and setters
	DrawableObject *setName(QString name);
    QString getName();
	int getType();
	DrawableObject *setId(int id);
	int getId();

	DrawableObject *setBrush(QBrush * value);
	QBrush * getBrush();
	DrawableObject *setPen(QPen * value);
	QPen * getPen();

	DrawableObject *setIsConstructional(bool value);
	bool isConstructional();
	DrawableObject *setHighlight(bool value);
	bool isHighlighted();
	DrawableObject *setHidden(bool value);
	bool isHidden();

	virtual DrawableObject *clone(){return nullptr;}

	//geometry
	void addGeometryUpdate(DrawableObject *object);
	void removeGeometryUpdate(DrawableObject *object);
	void removeGeometryUpdates();

    //array operator
	static DrawableObject *getById(QVector<DrawableObject*> *list, int id);

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   )override;

protected:
	DrawableObject(int type);

	//defining variables
	QString name;

	QBrush *brush;
	QPen *pen;

	//saving objects
	void fileAddVar(QString variable, QString value);
	void fileAddVar(QString variable, double value);
	void fileAddVar(QString variable, int value);
	void fileAddVar(QString variable, long value);
	void fileAddVar(QString variable, bool value);
	QString fileFinish();

	//loading objects
	QVector<QVariant> fetchVariables(QString input, QStringList varNames = QStringList());
	QVector<DrawableObject*> fetchRelations(QVector<DrawableObject*> *list, QStringList varNames = QStringList());

	//getters and setters
	QString getFile();
	bool isDraging();

	//events
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

	//geometry
	void updateGeometry();
	virtual void setGeometryUpdates(){}
	virtual void unsetGeometryUpdates(){}

private:
	//defining variables
	int id;
	int type;

	//parameters
	bool constructional = false;
	bool highlight = false;
	bool hidden = false;
	bool draging = false;

	//saving
	QString file;

	//geometry
	QVector<DrawableObject*> propagateGeometry;
};

#endif // DRAWABLEOBJECT_H

