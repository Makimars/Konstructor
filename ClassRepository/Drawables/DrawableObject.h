#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H


#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QtMath>
#include <QDebug>

#include "ClassRepository/Vector.h"
#include "ClassRepository/Settings.h"

class DrawableObject : public QGraphicsItem
{
public:
    DrawableObject();
    ~DrawableObject() override;

	bool is_constructional = false;
	bool highlight = false;

    virtual void resolveTies();

	//file handling
    virtual void fromFileString(QString input);
    virtual QString toFileString();
    virtual void loadRelations(QVector<DrawableObject*> *list) = 0;

	//getters and setters
    void setName(QString name);
    QString getName();
	QString getType();
    void setId(unsigned int id);
    unsigned int getId();

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


	//saving
	QString file;

};

#endif // DRAWABLEOBJECT_H

