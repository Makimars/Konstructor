#include "Settings.h"

//view widgets
double Settings::mouse_wheel_zoom_factor = 1.15;
bool Settings::mouse_wheel_inverted_zoom = false;
double Settings::point_snapping_distance = 10;

double  Settings::default_sketch_height = 4000;
double  Settings::default_sketch_width  = 4000;

int Settings::point_render_size = 2;

//environment
QString Settings::user_project_root = QDir::homePath() + "/Konstructor";
