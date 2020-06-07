#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include "Tool3D.h"

class DrawTool : public Tool3D
{
	Q_OBJECT
public:
	static DrawTool *getInstance();

	void click(QPoint pos) override;
	void resetTool() override;

private:
	DrawTool();
	static DrawTool *instance;

	QPointF planePosition;
	QVector3D planeVector;

signals:
	void requestDrawing();
	void addItem(Item *item);

public slots:
	void recieveDrawing(QVector<DrawableObject*> drawing);
};

#endif // DRAWTOOL_H
