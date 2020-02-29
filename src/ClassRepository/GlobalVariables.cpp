#include "GlobalVariables.h"

QString Global::openFile = "Open File";
QString Global::saveFile = "Save File";

QString Global::allFiles = "All files (*.*)";
QString Global::konstructorSketch = "Konstructor sketch .kosk (*.kosk)";

QStringList Global::toolNames = {
	"Line",
	"Circle",
	"Rectangle",
	"Label",
	"Dimension"
};

QStringList Global::typeNames = {
	TYPE_POINT,
	TYPE_LINE,
	TYPE_CIRCLE,
	TYPE_LINE_LENGTH_DIMENSION,
	TYPE_LINE_ANGLE_DIMENSION
};
