#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#define LINE_TOOL 0
#define CIRCLE_TOOL 1
#define RECTANGLE_TOOL 2
#define LABEL_TOOL 3
#define DIMENSION_TOOL 4

#include <QStringList>

#include "Tools/DimensionTool.h"

class Global {

public:
	//consatants
	static QStringList toolNames;
	static QStringList typeNames;

	//messages
	static QString openFile, saveFile;

	//file types
	static QString konstructorSketch;
	static QString allFiles;

};

#endif // GLOBALVARIABLES_H
