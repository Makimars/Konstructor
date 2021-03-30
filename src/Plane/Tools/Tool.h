#ifndef TOOL_H
#define TOOL_H

#include "../PlaneFactory.h"
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
	PlaneFactory *planeFactory;

	QStringList toolTips;
};

inline Tool::Tool()
{
	this->planeFactory = PlaneFactory::getInstance();
	clickCounter = 0;
}

inline QString Tool::getToolTip()
{
	if(clickCounter < toolTips.length())
			return toolTips.at(clickCounter);

	return "";
}


#endif // TOOL_H
