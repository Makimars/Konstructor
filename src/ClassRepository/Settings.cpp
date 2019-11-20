#include "Settings.h"

//view widgets
double Settings::mouseWheelZoomFactor = 1.15;
bool Settings::mouseWheelInvertedZoom = false;
double Settings::pointSnappingDistance = 10;

double  Settings::defaultSketchHeight = 4000;
double  Settings::defaultSketchWidth  = 4000;

int Settings::pointRenderSize = 2;

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
QString Settings::userProjectRoot = QDir::homePath() + "/Konstructor";
