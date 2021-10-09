#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>
#include <QKeySequence>
#include <QMargins>
#include <QLocale>
#include <QVector3D>
#include <QVector4D>

#include "include/nlohmann/json.hpp"

enum AngleUnits{
	radians,
	degrees
};

class Settings
{
public:
	static void fromJson(QString json);
	static QString toJson();

	//view widget
	static double mouseWheelZoomFactor;
	static bool mouseWheelInvertedZoom;

	static double sketchSize;

	static float defaultAngleDimensionTextDistance;

	static int pointRenderSize;
	static int lineShapeSize;
	static QMargins lineAngleArcMargins;
	static QMargins pointMargin;

	static bool gridSnapping;

	//keyboard shortcuts
	static QKeySequence
		newFile,
		openFile,
		saveFile,
		exportFile,
		openSettings,
		quitApp;

	//3D
	static int pointPerRadiusCoeficient;
	static bool colorByNormals;

	//environment
	static QString userProjectRoot;
	static QLocale locals;
	static AngleUnits angleUnits;

	static int planeToSpaceRatio;
	static int maxItemSidePlanes;

	static QVector3D selectedFaceColor;
	static QVector4D planeColor;
	static QVector4D lineColor;
};

#endif // SETTINGS_H
