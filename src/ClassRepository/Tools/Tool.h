#ifndef TOOL_H
#define TOOL_H

#include "../DrawablesFactory.h"

class Tool : public QObject
{
	Q_OBJECT
public:
	virtual void click(DrawableObject *clicked_object, Point *mouse_point){}
	virtual void resetTool();

	//getters and setters
	QBrush *getCurrentBrush() const;
	void setCurrentBrush(QBrush *value);
	QPen *getCurrentPen() const;
	void setCurrentPen(QPen *value);

private:

protected:
	Tool();

	DrawablesFactory *object_factory;

	QBrush *current_brush;
	QPen *current_pen;

};

#endif // TOOL_H
