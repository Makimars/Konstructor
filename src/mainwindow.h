#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>

#include "ClassRepository/Settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	//ui handeling
	void setTool(QString tool);

private:
	Ui::MainWindow *ui;
	int timerID;

	//settings
	void loadSettings();
	void saveSettings();

protected:
	void timerEvent(QTimerEvent *event);
private slots:

    //ui events
		//file tab
	void on_newButton_clicked();
	void on_openButton_clicked();
	void on_saveButton_clicked();
	void on_saveAsButton_clicked();
	void on_exportButton_clicked();
	void on_printButton_clicked();
	void on_settingsButton_clicked();
	void on_quitButton_clicked();
		//draw tab
	void on_lineButton_clicked();
	void on_circleButton_clicked();
	void on_rectangleButton_clicked();
	void on_labelButton_clicked();

	void viewKeyPress(QKeyEvent *event);
};

#endif // MAINWINDOW_H
