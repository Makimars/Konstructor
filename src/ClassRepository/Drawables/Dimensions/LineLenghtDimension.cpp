#include "LineLenghtDimension.h"

LineLenghtDimension::LineLenghtDimension(Line *line, double lenght)
{
	this->attachedLine = line;
	this->lengthToSet = lenght;
}

void LineLenghtDimension::resolveTies()
{
	this->attachedLine->setLength(this->lengthToSet);
}
