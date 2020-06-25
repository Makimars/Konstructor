#ifndef TRANSFERLINE_H
#define TRANSFERLINE_H

#include "TransferPoint.h"

class TransferLine
{
public:
	static TransferLine *fromDrawableLine(Line *line, QVector<TransferPoint*> *transferPointsList);

	TransferLine();
	TransferLine(TransferPoint *pointOne, TransferPoint *pointTwo);

	bool hasPoint(TransferPoint *point);
	TransferPoint *getOtherPoint(TransferPoint *point);

private:
	TransferPoint *points[2];
};

#endif // TRANSFERLINE_H
