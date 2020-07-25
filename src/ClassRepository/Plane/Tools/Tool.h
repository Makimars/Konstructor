#ifndef TOOL_H
#define TOOL_H

#include "../Factory.h"
#include <QKeyEvent>

namespace Plane {

class Tool : public QObject
{
	Q_OBJECT
public:
	virtual void click(DrawableObject *clickedObject, Point *mousePoint) = 0;
	virtual void resetTool() = 0;

	//getters and setters
	QBrush *getCurrentBrush() const;
	void setCurrentBrush(QBrush *value);
	QPen *getCurrentPen() const;
	void setCurrentPen(QPen *value);

protected:
	Factory *objectFactory;

	QBrush *currentBrush;
	QPen *currentPen;
};

inline QBrush *Tool::getCurrentBrush() const { return currentBrush; }
inline void Tool::setCurrentBrush(QBrush *value) { currentBrush = value; }
inline QPen *Tool::getCurrentPen() const { return currentPen; }
inline void Tool::setCurrentPen(QPen *value) { currentPen = value; }

}

#endif // TOOL_H
