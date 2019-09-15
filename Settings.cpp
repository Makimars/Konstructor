#include "Settings.h"

Settings * Settings::instance = new Settings();

Settings *Settings::getInstance()
{
	if(!Settings::instance)
		return new Settings();

	return Settings::instance;
}

void Settings::loadFromFile(QString file_content)
{

}

QString Settings::getSaveFile()
{

}

Settings::Settings()
{

}

Settings::~Settings()
{

}
