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

#include "ClassRepository/Space/Space"
#include "ClassRepository/Plane/Plane"
#include "ClassRepository/Adapters/Polygonator.h"

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
	QAction redrawAction;
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

	//file tab
	void on_newObjectFile_clicked();
	void on_openObjectFile_clicked();
	void on_saveObjectButton_clicked();
	void on_exportObjectButton_clicked();
	void on_settingsButton_clicked();
	void on_quitButton_clicked();
	//sketch files
	void on_saveSketchButton_clicked();
	void on_importSketchButton_clicked();
	void on_closeSketchButton_clicked();
	//sketch tools
	void on_lineButton_clicked();
	void on_circleButton_clicked();
	void on_rectangleButton_clicked();
	void on_labelButton_clicked();
	//constrains
	void on_dimensionButton_clicked();
	//finish drawing
	void on_finishDrawingButton_clicked();

	void on_objectsTree_itemDoubleClicked(QTreeWidgetItem *item, int column);
	void viewKeyPress(QKeyEvent *event);
	void swapMode(int index);
	void on_objectsTree_customContextMenuRequested(const QPoint &pos);
};

#endif // MAINWINDOW_H
