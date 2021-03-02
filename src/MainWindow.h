#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTreeWidgetItem>
#include <QMenuBar>

#include "Dialogs/WelcomeDialog.h"
#include "Dialogs/SettingsDialog.h"
#include "Dialogs/ExtrusionDialog.h"
#include "Base/GlobalVariables.h"

#include "Space/Space.h"
#include "Plane/Plane.h"
#include "Adapters/Polygonator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void show();

private:
	Ui::MainWindow *ui;
	SettingsDialog *settingsDialog;
	ExtrusionDialog *extrusionDialog;

    QMenu planeContextMenu;
    QAction drawAction;
    QMenu objectContextMenu;
	QAction redrawAction;
	QAction extrusionAction;
	QAction deleteAction;

	void setupUi();
	void setupConnections();

	//settings
	void loadSettings();
	void saveSettings();

	//Ui handeling
	void refreshTools(int tool);

signals:
	void setTool(int tool);
	void resetTool();

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
	void on_openSketchButton_clicked();
	void on_closeSketchButton_clicked();
	//sketch tools
	void on_lineButton_clicked();
	void on_circleButton_clicked();
	void on_rectangleButton_clicked();
	void on_labelButton_clicked();
	void on_expandPolygonButton_clicked();
	//constrains
	void on_pointPosButton_clicked();
	void on_lockPointButton_clicked();
	void on_pointDistanceConstraintButton_clicked();
	void on_circleRadiusConstraintButton_clicked();
	//finish drawing
	void on_finishDrawingButton_clicked();

	void on_objectsTree_itemDoubleClicked(QTreeWidgetItem *item, int column);
	void viewKeyPress(QKeyEvent *event);
	void setMode(int index);
	void on_objectsTree_customContextMenuRequested(const QPoint &pos);
	void on_arcButton_clicked();
	void on_objectsTree_itemClicked(QTreeWidgetItem *item, int column);

	std::vector<QPolygonF> getPolygonsForItem(QString sketch);
	void closeDrawing();
	void on_lineCenterButton_clicked();
	void on_paraelLineButton_clicked();
};

#endif // MAINWINDOW_H
