#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>

class Settings
{
public:
	static void loadFromFile(QString file_content);
	static QString getSaveFile();

	//view widget
	static double mouse_wheel_zoom_factor;
	static bool mouse_wheel_inverted_zoom;
	static double point_snapping_distance;

	static double default_sketch_height;
	static double default_sketch_width;

	static int point_render_size;

	//keyboard shortcuts

	//environment
	static QString user_project_root;

	//file extensions


private:
	Settings();
	~Settings();

};

#endif // SETTINGS_H
