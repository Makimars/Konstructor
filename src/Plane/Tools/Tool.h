#ifndef TOOL_H
#define TOOL_H

#include "../Factory.h"
#include <QKeyEvent>

class Tool : public QObject
{
	Q_OBJECT
public:
	virtual void click(DrawableObject *clickedObject, QPointF pos) = 0;
	virtual void resetTool(){};

	QString getToolTip();

public slots:
	virtual void mouseMoveEvent(QPointF){};

protected:
	Tool();

	int clickCounter;
	Factory *objectFactory;

	QStringList toolTips;
};

inline Tool::Tool()
{
	this->objectFactory = Factory::getInstance();
	clickCounter = 0;
}

inline QString Tool::getToolTip()
{
	if(clickCounter < toolTips.length())
			return toolTips.at(clickCounter);

	return "";
}


#endif // TOOL_H
