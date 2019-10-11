#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

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
