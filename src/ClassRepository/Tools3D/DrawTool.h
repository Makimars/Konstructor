#ifndef DRAWTOOL_H
#define DRAWTOOL_H

#include "Tool3D.h"

class DrawTool : public Tool3D
{
	Q_OBJECT
public:
	static DrawTool *getInstance();

	void click() override;
	void resetTool() override;

private:
	DrawTool();
	static DrawTool instance;

signals:
	void switchMode();
};

#endif // DRAWTOOL_H
