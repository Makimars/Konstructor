#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#define LINE_TOOL 0
#define CIRCLE_TOOL 1
#define RECTANGLE_TOOL 2
#define LABEL_TOOL 3

#include <QStringList>

class Global {

public:
	//consatants
    static QStringList tool_names;
    static QStringList type_names;

	//messages
    static QString open_file, save_file;

	//file types
    static QString konstructor_sketch;
    static QString all_files;

};

#endif // GLOBALVARIABLES_H
