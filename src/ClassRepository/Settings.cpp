#include "Settings.h"

//view widgets
double Settings::mouseWheelZoomFactor = 1.15;
bool Settings::mouseWheelInvertedZoom = false;
double Settings::snappingDistance = 10;

double  Settings::defaultSketchHeight = 4000;
double  Settings::defaultSketchWidth  = 4000;

float Settings::defaultAngleDimensionTextDistance = 200;


QMargins Settings::lineAngleArcMargins(10,10,10,10);
int Settings::pointRenderSize = 2;
QMargins Settings::pointMargin(2,2,2,2);

//keyboad shortcuts
QKeySequence Settings::newFile(Qt::CTRL + Qt::Key_N);
QKeySequence Settings::openFile(Qt::CTRL + Qt::Key_O);
QKeySequence Settings::saveFile(Qt::CTRL + Qt::Key_S);
QKeySequence Settings::saveFileAs(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
QKeySequence Settings::exportFile(Qt::CTRL + Qt::Key_E);
QKeySequence Settings::printFile(Qt::CTRL + Qt::Key_P);
QKeySequence Settings::openSettings(Qt::CTRL + Qt::SHIFT + Qt::Key_O);
QKeySequence Settings::quitApp(Qt::CTRL + Qt::Key_Q);

//environment
QString Settings::userProjectRoot = QDir::homePath();
QLocale Settings::locals(QLocale::English, QLocale::UnitedKingdom);
