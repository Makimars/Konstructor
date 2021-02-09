#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#define ZERO_POINT_ID -1
#define X_AXIS_ID -2
#define Y_AXIS_ID -3

#include <QString>
namespace Global
{

enum Types{
	Point,
	Line,
	Circle,
	Arc,
	Label,
	LineLengthDimension,
	LineAngleDimension,
	CircleRadiusDimension,
	CirclesRadiusDifferenceDimension,
	LineLengthConstraint
};

enum Tools{
	NoTool,
	LineTool,
	CircleTool,
	RectangleTool,
	LabelTool,
	DimensionTool,
	ArcTool,
	PointPositionTool,
	LockPointTool,
	PointDistanceConstrainTool,
	CircleRadiusConstraintTool,
	ExpandPolgyonTool,
	LineCenterTool
};

enum Mode{
	Draw,
	Object
};

	//messages
	const QString openFile = "Open File";
	const QString saveFile = "Save File";

	//file types
	const QString konstructorSketch = "Konstructor sketch (*.kosk)";
	const QString konstructorProject = "Konstructor project (*.kopr)";
	const QString allFiles;


}

#endif // GLOBALVARIABLES_H
