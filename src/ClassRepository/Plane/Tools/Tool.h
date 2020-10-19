#ifndef TOOL_H
#define TOOL_H

#include "../Factory.h"
#include <QKeyEvent>

class Tool : public QObject
{
	Q_OBJECT
public:
	virtual void click(DrawableObject *clickedObject, QPointF pos) = 0;
	virtual void resetTool() = 0;

public slots:
	virtual void mouseMoveEvent(QPointF pos) = 0;

protected:
	Tool();

	int clickCounter;
	Factory *objectFactory;

	//getters and setters
	void setCurrentBrush(QBrush *value);
	void setCurrentPen(QPen *value);
};

inline Tool::Tool()
{
	this->objectFactory = Factory::getInstance();
	clickCounter = 0;
}


#endif // TOOL_H
