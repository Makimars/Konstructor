#ifndef TOOL_H
#define TOOL_H

#include "../Factory.h"
#include <QKeyEvent>

class Tool : public QObject
{
	Q_OBJECT
public:
	virtual void click(DrawableObject *clickedObject, Point *mousePoint){}
	virtual void resetTool();

	//getters and setters
	QBrush *getCurrentBrush() const;
	void setCurrentBrush(QBrush *value);
	QPen *getCurrentPen() const;
	void setCurrentPen(QPen *value);

private:

protected:
	Tool();

	Factory *objectFactory;

	QBrush *currentBrush;
	QPen *currentPen;

};

#endif // TOOL_H
