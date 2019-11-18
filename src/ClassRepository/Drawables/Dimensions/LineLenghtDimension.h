#ifndef LINELENGHTDIMENSION_H
#define LINELENGHTDIMENSION_H

#include "../Circle.h"

class LineLenghtDimension : DrawableObject
{
public:
	LineLenghtDimension(Line *line, double lenght);

	void resolveTies() override;

private:
	Line *attached_line;
	double set_lenght;

};

#endif // LINELENGHTDIMENSION_H
