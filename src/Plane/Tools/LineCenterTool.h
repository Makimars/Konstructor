#ifndef LINECENTERTOOL_H
#define LINECENTERTOOL_H

#include "Tool.h"

class LineCenterTool : public Tool
{
public:
	static LineCenterTool *getInstance();

	LineCenterTool();
};

#endif // LINECENTERTOOL_H
