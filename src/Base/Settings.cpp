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
int Settings::pointPerRadiusCoeficient = 2;
bool Settings::colorByNormals = true;

//environment
QString Settings::userProjectRoot = QDir::homePath();
QLocale Settings::locals(QLocale::English, QLocale::UnitedKingdom);
AngleUnits Settings::angleUnits = AngleUnits::degrees;
int Settings::planeToSpaceRatio = 25;
int Settings::maxItemSidePlanes = 12;

QVector3D Settings::selectedFaceColor = QVector3D(0.1, 0.1, 1);
QVector4D Settings::planeColor = QVector4D(1, 0, 0, 0.3);
QVector4D Settings::lineColor = QVector4D(1, 0, 1, 0);

void Settings::fromJson(QString json)
{
	nlohmann::json input = nlohmann::json::parse(json.toStdString());

	Settings::mouseWheelInvertedZoom = input["mouseWheelInvertedZoom"];
	Settings::gridSnapping = input["gridSnapping"];
	Settings::colorByNormals = input["colorByNormals"];

	double pointSize =  input["pointSize"];
	Settings::pointMargin = QMargins(pointSize,pointSize,pointSize,pointSize);
	Settings::lineShapeSize = input["lineShapeSize"];

	Settings::sketchSize = input["sketchSize"];
	Settings::maxItemSidePlanes = input["maxItemSidePlanes"];

	Settings::angleUnits = input["angleUnits"];

	Settings::newFile = QKeySequence::fromString(QString::fromStdString(input["newFile"]));
	Settings::openFile = QKeySequence::fromString(QString::fromStdString(input["openFile"]));
	Settings::saveFile = QKeySequence::fromString(QString::fromStdString(input["saveFile"]));
	Settings::exportFile = QKeySequence::fromString(QString::fromStdString(input["exportFile"]));
	Settings::openSettings = QKeySequence::fromString(QString::fromStdString(input["openSettings"]));

	double x,y,z,w;
	x = input["planeColorX"];
	y = input["planeColorY"];
	z = input["planeColorZ"];
	w = input["planeColorW"];

	Settings::planeColor = QVector4D(x, y, z, w);
}

QString Settings::toJson()
{
	nlohmann::json json;

	json["mouseWheelInvertedZoom"] = Settings::mouseWheelInvertedZoom;
	json["gridSnapping"] = Settings::gridSnapping;
	json["pointSize"] = Settings::pointMargin.top();
	json["colorByNormals"] = Settings::colorByNormals;

	json["sketchSize"] = Settings::sketchSize;
	json["lineShapeSize"] = Settings::lineShapeSize;
	json["maxItemSidePlanes"] = Settings::maxItemSidePlanes;
	json["angleUnits"] = Settings::angleUnits;

	json["planeColorX"] = Settings::planeColor.x();
	json["planeColorY"] = Settings::planeColor.y();
	json["planeColorZ"] = Settings::planeColor.z();
	json["planeColorW"] = Settings::planeColor.w();

	json["newFile"] = Settings::newFile.toString().toStdString();
	json["openFile"] = Settings::openFile.toString().toStdString();
	json["saveFile"] = Settings::saveFile.toString().toStdString();
	json["exportFile"] = Settings::exportFile.toString().toStdString();
	json["openSettings"] = Settings::openSettings.toString().toStdString();

	return QString::fromStdString(json.dump());
}
