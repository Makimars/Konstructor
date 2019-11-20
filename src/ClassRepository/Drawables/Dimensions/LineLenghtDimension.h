#ifndef LINELENGHTDIMENSION_H
#define LINELENGHTDIMENSION_H

#include "../Circle.h"

class LineLenghtDimension : DrawableObject
{
public:
	LineLenghtDimension(Line *line, double lenght);

	void resolveTies() override;

private:
	Line *attachedLine;
	double lengthToSet;

};

#endif // LINELENGHTDIMENSION_H
