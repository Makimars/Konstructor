#ifndef LABELTOOL_H
#define LABELTOOL_H

#include "RectangleTool.h"

class LabelTool : public Tool
{
public:
	static void initialise(Point *mouse_point,
						   QGraphicsScene *scene,
						   QBrush *default_brush,
						   QPen *default_pen
						   );
	static LabelTool *getInstance();

	void click(Point *clicked_point, bool existing_point = false) override;
	void resetTool() override;

private:
	LabelTool(Point *mouse_point, QGraphicsScene *scene);

	static LabelTool *instance;
};

#endif // LABELTOOL_H
