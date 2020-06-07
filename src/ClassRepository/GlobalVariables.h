#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#define ZERO_POINT_ID -1
#define X_AXIS_ID -2
#define Y_AXIS_ID -3

#include <QStringList>
namespace Global
{

enum Types{
	Type_Point,
	Type_Line,
	Type_Circle,
	Type_Label,
	Type_LineLengthDimension,
	Type_LineAngleDimension,
	Type_CircleRadiusDimension,
	Type_CirclesRadiusDifferenceDimension
};

enum Tools{
	None,
	Line,
	Circle,
	Rectangle,
	Label,
	Dimension,
	Draw
};

enum Mode{
	object,
	draw
};

	//messages
	const QString openFile = "Open File";
	const QString saveFile = "Save File";

	//file types
	const QString konstructorSketch;
	const QString allFiles;

}


#endif // GLOBALVARIABLES_H
