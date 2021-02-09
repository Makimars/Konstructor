#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H


#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QtMath>
#include <QVector2D>

#include <QDebug>

#include "src/Base/Settings.h"
#include "src/Base/GlobalVariables.h"
#include "../UserInputRequester.h"
#include "../Style.h"

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
	int getType();
	void setId(int id);
	int getId();
	void setLocked(bool value);
	bool isLocked();
	void addConstrant();
	void removeConstraint();
	bool isConstrained();

	void setStyle(const Style *style);

	void setIsConstructional(bool value);
	bool isConstructional();
	void setHighlight(bool value);
	bool isHighlighted();
	void setHidden(bool value);
	bool isHidden();

	//geometry
	void addGeometryUpdate(DrawableObject *object);
	void removeGeometryUpdate(DrawableObject *object);
	void removeGeometryUpdates();

protected:
	DrawableObject(int type);

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

	void paint(QPainter *painter);

private:
	const Style *style;

	//defining variables
	int id;
	int type;

	//parameters
	bool constructional = false;
	bool highlight = false;
	bool hidden = false;
	bool draging = false;
	bool locked = false;
	int constrains = 0;

	//saving
	QString file;

	//geometry
	QVector<DrawableObject*> propagateGeometry;

	//array operator
	static DrawableObject *getById(QVector<DrawableObject*> *list, int id);
};

//inline getters and setters
inline int DrawableObject::getType() { return type; }
inline void DrawableObject::setId(int id) { this->id = id; }
inline int DrawableObject::getId() { return id; }
inline void DrawableObject::setLocked(bool value) { locked = value; }
inline bool DrawableObject::isLocked() { return locked | (id < 0); }

inline void DrawableObject::setStyle(const Style *style) { this->style = style; }
inline void DrawableObject::setIsConstructional(bool value) { constructional = value; }
inline bool DrawableObject::isConstructional() { return constructional; }
inline void DrawableObject::setHidden(bool value) { hidden = value; }
inline bool DrawableObject::isHidden() {return hidden; }
inline void DrawableObject::setHighlight(bool value) { highlight = value; }
inline bool DrawableObject::isHighlighted() { return highlight; }
inline bool DrawableObject::isDraging() { return draging; }

//inline fileAddVar

inline void DrawableObject::fileAddVar(QString variable, QString value)
{ this->file += variable + ":" + value + ","; }

inline void DrawableObject::fileAddVar(QString variable, double value)
{ this->fileAddVar(variable, QString::number(value)); }

inline void DrawableObject::fileAddVar(QString variable, int value)
{ this->fileAddVar(variable, QString::number(value)); }

inline void DrawableObject::fileAddVar(QString variable, long value)
{ this->fileAddVar(variable, QString::number(value)); }

inline void DrawableObject::fileAddVar(QString variable, bool value)
{ this->fileAddVar(variable, QString::number(value)); }

#endif // DRAWABLEOBJECT_H

