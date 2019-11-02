#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H


#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QtMath>
#include <QDebug>

#include "src/ClassRepository/Vector.h"
#include "src/ClassRepository/Settings.h"

class DrawableObject : public QGraphicsItem
{
public:
    DrawableObject();
    ~DrawableObject() override;

    virtual void resolveTies();

	//file handling

	/**
	 * @brief asigns variables from a file representation
	 * @param input
	 */
    virtual void fromFileString(QString input);

	/**
	 * @brief generates file representation of an object
	 * @return file
	 */
    virtual QString toFileString();

	/**
	 * @brief assign pointers to objects to a variable from a vector of drawables
	 * @param list
	 */
    virtual void loadRelations(QVector<DrawableObject*> *list) = 0;

	//getters and setters
    void setName(QString name);
    QString getName();
	QString getType();
    void setId(unsigned int id);
    unsigned int getId();

	void setIsConstructiona(bool value);
	bool isConstructional();
	void setHighlight(bool value);
	bool isHighlighted();
	void setHidden(bool value);
	bool isHidden();

	virtual DrawableObject *Clone(){}

	//saving
	void fileAddVar(QString variable, QString value);
	void fileAddVar(QString variable, double value);
	void fileAddVar(QString variable, int value);
	void fileAddVar(QString variable, long value);
    void fileAddVar(QString variable, unsigned int value);
	void fileAddVar(QString variable, bool value);
	QString fileFinish();

    //array operator
	static DrawableObject *getById(QVector<DrawableObject*> *list, unsigned int id);

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   )override;

protected:
    QString type, name;
    unsigned int id;

	bool is_constructional = false;
	bool highlight = false;
	bool hidden = false;

	//saving
	QString file;

};

#endif // DRAWABLEOBJECT_H

