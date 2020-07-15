#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include "src/ClassRepository/Plane/Drawables/Dimensions/CirclesRadiusDifferenceDimension.h"
#include "../Item.h"

class DrawTool : public QObject
{
	Q_OBJECT
public:
	static DrawTool *getInstance();

private:
	DrawTool();
	static DrawTool *instance;

signals:
	void requestDrawing();
	void addItem(Item *item);
	QTreeWidgetItem *getPlane();

public slots:
	void recieveDrawing(QVector<DrawableObject*> drawing);
};

#endif // DRAWTOOL_H
