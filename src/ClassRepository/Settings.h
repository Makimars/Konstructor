#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>
#include <QKeySequence>
#include <QMargins>
#include <QLocale>


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
		saveFileAs,
		exportFile,
		printFile,
		openSettings,
		quitApp;


	//environment
	static QString userProjectRoot;
	static QLocale locals;
	static AngleUnits angleUnits;


private:
	Settings();
	~Settings();

};

#endif // SETTINGS_H
