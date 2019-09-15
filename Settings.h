#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QDir>

class Settings
{
public:
	static Settings * getInstance();

    void loadFromFile(QString file_content);
    QString getSaveFile();

	//settings variables

		//view widget
	double mouse_wheel_zoom_factor = 1.2;
	bool mouse_wheel_inverted_zoom = false;

		//keyboard shortcuts

		//environment
	QString user_project_root = QDir::homePath() + "/Konstructor";

        //file extensions

private:
	Settings();
	~Settings();

	static Settings * instance;

};

#endif // SETTINGS_H
