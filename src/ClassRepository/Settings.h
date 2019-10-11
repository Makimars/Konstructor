#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>
#include <QKeySequence>

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
	static QKeySequence
		new_file,
		open_file,
		save_file,
		save_file_as,
		export_file,
		print_file,
		open_settings,
		quit_app;


	//environment
	static QString user_project_root;

	//file extensions


private:
	Settings();
	~Settings();

};

#endif // SETTINGS_H
