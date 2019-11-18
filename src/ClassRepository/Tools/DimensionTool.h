#ifndef DIMENSIONSTOOL_H
#define DIMENSIONSTOOL_H

#include "LabelTool.h"

class DimensionTool : public Tool
{
public:
	static void initialise(Point *mouse_point,
						   QGraphicsScene *scene,
						   QBrush *default_brush,
						   QPen *default_pen
						   );
	static DimensionTool *getInstance();

	void click(DrawableObject *clicked_object, Point *mouse_point) override;
	void resetTool() override;

private:
	DimensionTool(Point *mouse_point, QGraphicsScene *scene);
	static DimensionTool *instance;

	//working variables
	DrawableObject *clicked_objects[2] = {nullptr, nullptr};

};

#endif // DIMENSIONSTOOL_H
