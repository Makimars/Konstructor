#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "Settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow * ui;
	int timerID;

	//settings
	void loadSettings();
	void saveSettings();

	Settings * settings;

protected:
	void timerEvent(QTimerEvent *event);
private slots:

    //ui events
        //file tab
	void on_new_button_clicked();
	void on_open_button_clicked();
	void on_save_button_clicked();
	void on_save_as_button_clicked();
	void on_export_button_clicked();
	void on_print_button_clicked();
	void on_settings_button_clicked();
	void on_quit_button_clicked();
        //draw tab

};

#endif // MAINWINDOW_H
