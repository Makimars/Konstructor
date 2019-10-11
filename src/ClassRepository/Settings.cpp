#include "Settings.h"

//view widgets
double Settings::mouse_wheel_zoom_factor = 1.15;
bool Settings::mouse_wheel_inverted_zoom = false;
double Settings::point_snapping_distance = 10;

double  Settings::default_sketch_height = 4000;
double  Settings::default_sketch_width  = 4000;

int Settings::point_render_size = 2;

//keyboad shortcuts
QKeySequence Settings::new_file(Qt::CTRL + Qt::Key_N);
QKeySequence Settings::open_file(Qt::CTRL + Qt::Key_O);
QKeySequence Settings::save_file(Qt::CTRL + Qt::Key_S);
QKeySequence Settings::save_file_as(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
QKeySequence Settings::export_file(Qt::CTRL + Qt::Key_E);
QKeySequence Settings::print_file(Qt::CTRL + Qt::Key_P);
QKeySequence Settings::open_settings(Qt::CTRL + Qt::SHIFT + Qt::Key_O);
QKeySequence Settings::quit_app(Qt::CTRL + Qt::Key_Q);

//environment
QString Settings::user_project_root = QDir::homePath() + "/Konstructor";
