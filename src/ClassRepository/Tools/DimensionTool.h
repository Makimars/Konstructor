#ifndef DIMENSIONSTOOL_H
#define DIMENSIONSTOOL_H

#include "LabelTool.h"

class DimensionTool : public Tool
{
public:
	static void initialise(Point *mousePoint,
						   QGraphicsScene *scene,
						   QBrush *defaultBrush,
						   QPen *defaultPen
						   );
	static DimensionTool *getInstance();

	void click(DrawableObject *clickedObject, Point *mousePoint) override;
	void resetTool() override;

private:
	DimensionTool(Point *mousePoint, QGraphicsScene *scene);
	static DimensionTool *instance;

	//working variables
	DrawableObject *clickedObjects[2] = {nullptr, nullptr};

	DrawableObject *dimensionPreview = nullptr;
};

#endif // DIMENSIONSTOOL_H
