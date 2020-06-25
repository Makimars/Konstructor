#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include "Tool.h"

class DrawTool : public Space::Tool
{
	Q_OBJECT
public:
	static DrawTool *getInstance();

	void click(QPoint pos) override;
	void resetTool() override;

private:
	DrawTool();
	static DrawTool *instance;

	QVector3D planePosition;
	QVector3D planeVector;

signals:
	void requestDrawing();
	void addItem(Item *item);

public slots:
	void recieveDrawing(QVector<DrawableObject*> drawing);
};

#endif // DRAWTOOL_H
