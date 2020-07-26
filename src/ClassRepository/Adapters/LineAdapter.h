#ifndef TRANSFERLINE_H
#define TRANSFERLINE_H

#include "PointAdapter.h"

class LineAdapter
{
public:
	static LineAdapter *fromDrawableLine(Line *line, QVector<PointAdapter*> *transferPointsList);

	LineAdapter();
	LineAdapter(PointAdapter *pointOne, PointAdapter *pointTwo);

	bool hasPoint(PointAdapter *point);
	PointAdapter *getOtherPoint(PointAdapter *point);

private:
	PointAdapter *points[2];
};

#endif // TRANSFERLINE_H
