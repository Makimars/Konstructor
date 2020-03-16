#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>
#include <QKeySequence>
#include <QMargins>

class Settings
{
public:
	static void loadFromFile(QString fileContent);
	static QString getSaveFile();

	//view widget
	static double mouseWheelZoomFactor;
	static bool mouseWheelInvertedZoom;
	static double snappingDistance;

	static double defaultSketchHeight;
	static double defaultSketchWidth;

	static float defaultAngleDimensionTextDistance;

	static int pointRenderSize;
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

	//file extensions


private:
	Settings();
	~Settings();

};

#endif // SETTINGS_H
