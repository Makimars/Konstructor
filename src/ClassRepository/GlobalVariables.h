#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#define LINE_TOOL 0
#define CIRCLE_TOOL 1
#define RECTANGLE_TOOL 2
#define LABEL_TOOL 3
#define DIMENSION_TOOL 4

#include <QStringList>

enum Types{
	Type_Point,
	Type_Line,
	Type_Circle,
	Type_Label,
	Type_LineLengthDimension,
	Type_LineAngleDimension,
	Type_CircleRadiusDimension
};

class Global {

public:
	//consatants
	static QStringList toolNames;

	//messages
	static QString openFile, saveFile;

	//file types
	static QString konstructorSketch;
	static QString allFiles;

};

#endif // GLOBALVARIABLES_H
