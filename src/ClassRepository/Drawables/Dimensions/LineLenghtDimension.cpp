#include "LineLenghtDimension.h"

LineLenghtDimension::LineLenghtDimension(Line *line, double lenght)
{
	this->attached_line = line;
	this->set_lenght = lenght;
}

void LineLenghtDimension::resolveTies()
{
	this->attached_line->setLength(this->set_lenght);
}
