#include "Settings.h"

//view widgets
double Settings::mouseWheelZoomFactor = 1.15;
bool Settings::mouseWheelInvertedZoom = false;
double Settings::snappingDistance = 10;

double Settings::sketchSize  = 4000;

float Settings::defaultAngleDimensionTextDistance = 200;


int Settings::pointRenderSize = 2;
int Settings::lineShapeSize = 5;
QMargins Settings::lineAngleArcMargins(10,10,10,10);
QMargins Settings::pointMargin(2,2,2,2);

//keyboad shortcuts
QKeySequence Settings::newFile(Qt::CTRL + Qt::Key_N);
QKeySequence Settings::openFile(Qt::CTRL + Qt::Key_O);
QKeySequence Settings::saveFile(Qt::CTRL + Qt::Key_S);
QKeySequence Settings::exportFile(Qt::CTRL + Qt::Key_E);
QKeySequence Settings::openSettings(Qt::CTRL + Qt::SHIFT + Qt::Key_O);
QKeySequence Settings::quitApp(Qt::CTRL + Qt::Key_Q);

//environment
QString Settings::userProjectRoot = QDir::homePath();
QLocale Settings::locals(QLocale::English, QLocale::UnitedKingdom);
AngleUnits Settings::angleUnits = AngleUnits::degrees;
int Settings::planeToSpaceRatio = 50;
QVector3D Settings::selectedFaceColor = QVector3D(0.1, 0.1, 1);
