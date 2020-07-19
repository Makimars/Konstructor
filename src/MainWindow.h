#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTreeWidgetItem>
#include <QMenuBar>

#include "SettingsDialog.h"
#include "ExtrusionDialog.h"

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
	ExtrusionDialog *extrusionDialog;

    QMenu planeContextMenu;
    QAction drawAction;
    QMenu objectContextMenu;
    QAction extrusionAction;

	void setupUi();

	//settings
	void loadSettings();
	void saveSettings();

	//Ui handeling
	void refreshTools(int tool);

signals:
	void setTool(int tool);
	void resetTool();

	void finishDrawing();
	void setTargetItem(QTreeWidgetItem *item);

private slots:

    //ui events
		//file tab
	void newFileClicked();
	void openFileClicked();
	void saveFileClicked();
	void exportFileClicked();
	void settingsClicked();
	void quitClicked();
		//draw tab
	void on_lineButton_clicked();
	void on_circleButton_clicked();
	void on_rectangleButton_clicked();
	void on_labelButton_clicked();
	void on_dimensionButton_clicked();
	void on_finishDrawingButton_clicked();

	void on_objectsTree_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void openExtrusion();
    void openDrawing();

	void viewKeyPress(QKeyEvent *event);
	void swapMode(int index);
    void on_objectsTree_customContextMenuRequested(const QPoint &pos);
};

#endif // MAINWINDOW_H
