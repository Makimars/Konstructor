#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include "include/delaunator/delaunator.h"

#include "../Item.h"
#include "../../Settings.h"

class DrawTool : public QObject
{
	Q_OBJECT
public:
	static DrawTool *getInstance();

private:
	DrawTool();
	static DrawTool *instance;

	QTreeWidgetItem *targetItem;

	std::vector<Vertex> generateVertexes(QPolygonF polygon, std::vector<Vertex> *outerVertexes);

signals:
	void addItem(Item *item);
	QTreeWidgetItem *getPlane();

public slots:
	void recieveTargetItem(QTreeWidgetItem *item);
	void recievePolygons(std::vector<QPolygonF> polygons, QString sketch);
};

inline void DrawTool::recieveTargetItem(QTreeWidgetItem *item) { targetItem = item;}

#endif // DRAWTOOL_H
