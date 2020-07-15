#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTreeWidgetItem>

#include "SettingsDialog.h"
#include "ClassRepository/GlobalVariables.h"
#include "ClassRepository/Space/Plane.h"

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
	Ui::MainWindow *ui;
	SettingsDialog *settingsDialog;

	//settings
	void loadSettings();
	void saveSettings();

	//Ui handeling
	void refreshTools(int tool);
signals:
	void setTool(int tool);
	void resetTool();

	void finishDrawing();

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
	void on_dimensionButton_clicked();
		//object tab
	void on_drawButton_clicked();
	void on_finishDrawingButton_clicked();

	void viewKeyPress(QKeyEvent *event);

	void swapMode(int index);
	QTreeWidgetItem *getSelectedPlane();
};

#endif // MAINWINDOW_H
