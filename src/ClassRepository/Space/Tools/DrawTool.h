#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include "include/delaunator/delaunator.h"

#include "../Item.h"

class DrawTool : public QObject
{
	Q_OBJECT
public:
	static DrawTool *getInstance();

private:
	DrawTool();
	static DrawTool *instance;

	QTreeWidgetItem *targetItem;

	std::vector<Vertex> generateVertexes(QVector<DrawableObject*> drawing);
		void convertToIntermediate(QVector<DrawableObject*> drawing, QVector<TransferPoint*> *transferPoints, QVector<TransferLine*> *transferLines);
		void assignNeigbors(QVector<TransferPoint*> *transferPoints, QVector<TransferLine*> *transferLines);
		std::vector<Vertex> triangularize(QVector<TransferPoint*> *transferPoints);

signals:
	void addItem(Item *item);
	QTreeWidgetItem *getPlane();

public slots:
	void recieveTargetItem(QTreeWidgetItem *item);
	void recieveDrawing(QVector<DrawableObject*> drawing);
};

inline void DrawTool::recieveTargetItem(QTreeWidgetItem *item) { targetItem = item;}

#endif // DRAWTOOL_H
