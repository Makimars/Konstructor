#include "Settings.h"

//view widgets
double Settings::mouse_wheel_zoom_factor = 1.2;
bool Settings::mouse_wheel_inverted_zoom = false;

//environment
QString Settings::user_project_root = QDir::homePath() + "/Konstructor";
