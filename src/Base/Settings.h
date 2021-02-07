#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>
#include <QKeySequence>
#include <QMargins>
#include <QLocale>
#include <QVector3D>
#include <QVector4D>

enum AngleUnits{
	radians,
	degrees
};

class Settings
{
public:
	static void loadFromFile(QString fileContent);
	static QString getSaveFile();

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

	//environment
	static QString userProjectRoot;
	static QLocale locals;
	static AngleUnits angleUnits;

	static int planeToSpaceRatio;
	static int maxItemSidePlanes;

	static QVector3D selectedFaceColor;
	static QVector4D planeColor;
};

#endif // SETTINGS_H
