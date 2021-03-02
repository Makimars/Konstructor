#include "Settings.h"

//view widgets
double Settings::mouseWheelZoomFactor = 1.15;
bool Settings::mouseWheelInvertedZoom = false;

double Settings::sketchSize  = 4000;

float Settings::defaultAngleDimensionTextDistance = 200;

int Settings::pointRenderSize = 4;
int Settings::lineShapeSize = 3;
QMargins Settings::lineAngleArcMargins(10,10,10,10);
QMargins Settings::pointMargin(4,4,4,4);

bool Settings::gridSnapping = true;

//keyboad shortcuts
QKeySequence Settings::newFile(Qt::CTRL + Qt::Key_N);
QKeySequence Settings::openFile(Qt::CTRL + Qt::Key_O);
QKeySequence Settings::saveFile(Qt::CTRL + Qt::Key_S);
QKeySequence Settings::exportFile(Qt::CTRL + Qt::Key_E);
QKeySequence Settings::openSettings(Qt::CTRL + Qt::SHIFT + Qt::Key_O);
QKeySequence Settings::quitApp(Qt::CTRL + Qt::Key_Q);

//3D
int Settings::pointPerRadiusCoeficient = 10;

//environment
QString Settings::userProjectRoot = QDir::homePath();
QLocale Settings::locals(QLocale::English, QLocale::UnitedKingdom);
AngleUnits Settings::angleUnits = AngleUnits::degrees;
int Settings::planeToSpaceRatio = 50;
int Settings::maxItemSidePlanes = 12;

QVector3D Settings::selectedFaceColor = QVector3D(0.1, 0.1, 1);
QVector4D Settings::planeColor = QVector4D(1, 0, 0, 0.3);