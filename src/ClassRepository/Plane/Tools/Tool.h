#ifndef TOOL_H
#define TOOL_H

#include "../Factory.h"
#include <QKeyEvent>

class Tool : public QObject
{
	Q_OBJECT
public:
	virtual void click(DrawableObject *clickedObject, QPointF pos);
	virtual void resetTool();

protected:
	Tool();

	Factory *objectFactory;

	//getters and setters
	void setCurrentBrush(QBrush *value);
	void setCurrentPen(QPen *value);
};

inline Tool::Tool()
{
	this->objectFactory = Factory::getInstance();
}


#endif // TOOL_H
