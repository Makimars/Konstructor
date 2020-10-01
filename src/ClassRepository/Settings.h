#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>
#include <QKeySequence>
#include <QMargins>
#include <QLocale>
#include <QVector3D>

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
	static double snappingDistance;

	static double sketchSize;

	static float defaultAngleDimensionTextDistance;

	static int pointRenderSize;
	static int lineShapeSize;
	static QMargins lineAngleArcMargins;
	static QMargins pointMargin;

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

	static QVector3D selectedFaceColor;

private:
	Settings();
	~Settings();

};

#endif // SETTINGS_H
