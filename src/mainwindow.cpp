#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ClassRepository/GlobalVariables.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	this->ui->setupUi(this);
	this->settingsDialog = new SettingsDialog();

	loadSettings();

	this->ui->newButton->setShortcut(Settings::newFile);
	this->ui->openButton->setShortcut(Settings::openFile);
	this->ui->saveButton->setShortcut(Settings::saveFile);
	this->ui->saveAsButton->setShortcut(Settings::saveFileAs);
	this->ui->exportButton->setShortcut(Settings::exportFile);
	this->ui->printButton->setShortcut(Settings::printFile);
	this->ui->settingsButton->setShortcut(Settings::openSettings);
	this->ui->quitButton->setShortcut(Settings::quitApp);

	connect(this->ui->view2D, &ViewWidget::keyPressed,
			this, &MainWindow::viewKeyPress
			);
	connect(this, &MainWindow::setTool,
			this->ui->view2D, &ViewWidget::setTool
			);
	connect(this, &MainWindow::resetTool,
			this->ui->view2D, &ViewWidget::resetTool
			);
	connect(this->ui->view2D, &ViewWidget::showStatusBarMessage,
			this->ui->statusBar, &QStatusBar::showMessage
			);
}

MainWindow::~MainWindow()
{
	delete this->ui;
}

//----------    Ui handeling    ---------

void MainWindow::refreshTools(QString toolName)
{
	if(toolName != "Line")
		this->ui->lineButton->setChecked(false);
	if(toolName != "Circle")
		this->ui->circleButton->setChecked(false);
	if(toolName != "Rectangle")
		this->ui->rectangleButton->setChecked(false);
	if(toolName != "Label")
		this->ui->labelButton->setChecked(false);
	if(toolName != "Dimension")
		this->ui->dimensionButton->setChecked(false);

	emit setTool(toolName);
}

//----------	settings    ----------

void MainWindow::loadSettings()
{

}

void MainWindow::saveSettings()
{

}


//----------	ui signals reciever    ----------

//-----    file tab    -----

void MainWindow::on_newButton_clicked()
{
	this->ui->view2D->newFile();
}

void MainWindow::on_openButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(
			this,
					Global::openFile,
			Settings::userProjectRoot,
			Global::konstructorSketch + ";;" + Global::allFiles
			);

	QFile file(fileName);
	if(file.exists())
	{
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
			this->ui->view2D->loadFromFile(file.readAll());
	}
}

void MainWindow::on_saveButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(
			this,
			Global::saveFile,
			Settings::userProjectRoot,
			Global::konstructorSketch + ";;" + Global::allFiles
			);
	this->ui->view2D->saveToFile(fileName);
}

void MainWindow::on_saveAsButton_clicked()
{

}

void MainWindow::on_exportButton_clicked()
{

}

void MainWindow::on_printButton_clicked()
{

}

void MainWindow::on_settingsButton_clicked()
{
	this->settingsDialog->exec();
}

void MainWindow::on_quitButton_clicked()
{
	this->close();
}

//-----    draw tab    -----

void MainWindow::on_lineButton_clicked()
{
	if(this->ui->lineButton->isChecked())
		refreshTools("Line");
	else
		refreshTools("");
}

void MainWindow::on_circleButton_clicked()
{

	if(this->ui->circleButton->isChecked())
		refreshTools("Circle");
	else
		refreshTools("");
}

void MainWindow::on_rectangleButton_clicked()
{
	if(this->ui->rectangleButton->isChecked())
		refreshTools("Rectangle");
	else
		refreshTools("");
}

void MainWindow::on_labelButton_clicked()
{
	if(this->ui->labelButton->isChecked())
		refreshTools("Label");
	else
		refreshTools("");
}

void MainWindow::on_dimensionButton_clicked()
{
	if(this->ui->dimensionButton->isChecked())
		refreshTools("Dimension");
	else
		refreshTools("");
}

void MainWindow::viewKeyPress(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key::Key_Escape:
			refreshTools("");
			break;
		case Qt::Key::Key_Enter:
			emit resetTool();
			break;
	}

}
