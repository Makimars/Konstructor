#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H


#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QtMath>
#include <QVector2D>

#include "include/nlohmann/json.hpp"

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
	virtual void loadData(nlohmann::json jsonInput);
	/**
	 * @brief assign pointers to objects to a variable from a vector of drawables
	 * @param list
	 */
	virtual void loadRelations(QVector<DrawableObject*> list){};
	/**
	 * @brief generates file representation of an object
	 * @return json
	 */
	virtual nlohmann::json toJson();

	//getters and setters
	int getType();
	void setId(int id);
	int getId();
	void setLocked(bool value);
	bool isLocked();
	void addConstraint();
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
	bool hasGeometryUpdates();
	void addGeometryUpdate(DrawableObject *object);
	void removeGeometryUpdate(DrawableObject *object);
	void removeGeometryUpdates();

protected:
	DrawableObject(int type);

	//loading objects
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

	//saving objects
	nlohmann::json json;

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

#endif // DRAWABLEOBJECT_H

