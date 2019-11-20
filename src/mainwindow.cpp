#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ClassRepository/GlobalVariables.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	this->ui->setupUi(this);
	this->timerID = startTimer(50);

	loadSettings();

	this->ui->newButton->setShortcut(Settings::newFile);
	this->ui->openButton->setShortcut(Settings::openFile);
	this->ui->saveButton->setShortcut(Settings::saveFile);
	this->ui->saveAsButton->setShortcut(Settings::saveFileAs);
	this->ui->exportButton->setShortcut(Settings::exportFile);
	this->ui->printButton->setShortcut(Settings::printFile);
	this->ui->settingsButton->setShortcut(Settings::openSettings);
	this->ui->quitButton->setShortcut(Settings::quitApp);

	connect(this->ui->mainViewWidget, &ViewWidget::keyPressed,
			this, &MainWindow::viewKeyPress
			);
}

MainWindow::~MainWindow()
{
	delete this->ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	this->ui->mainViewWidget->update();
}

//----------    Ui handeling    ---------

void MainWindow::setTool(QString tool)
{
	if(tool != "Line")
		this->ui->lineButton->setChecked(false);
	if(tool != "Circle")
		this->ui->circleButton->setChecked(false);
	if(tool != "Rectangle")
		this->ui->rectangleButton->setChecked(false);
	if(tool != "Label")
		this->ui->labelButton->setChecked(false);

	this->ui->mainViewWidget->setTool(tool);
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
			this->ui->mainViewWidget->loadFromFile(file.readAll());
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
	this->ui->mainViewWidget->saveToFile(fileName);
}

void MainWindow::on_saveAsButton_clicked()
{

}

void MainWindow::on_exportButton_clicked()
{

}

void MainWindow::on_printButton_clicked()
{
	this->close();
}

void MainWindow::on_settingsButton_clicked()
{

}

void MainWindow::on_quitButton_clicked()
{

}

//-----    draw tab    -----

void MainWindow::on_lineButton_clicked()
{
	if(this->ui->lineButton->isChecked())
		setTool("Line");
	else
		setTool("");
}

void MainWindow::on_circleButton_clicked()
{

	if(this->ui->circleButton->isChecked())
		setTool("Circle");
	else
		setTool("");
}

void MainWindow::on_rectangleButton_clicked()
{
	if(this->ui->rectangleButton->isChecked())
		setTool("Rectangle");
	else
		setTool("");
}

void MainWindow::on_labelButton_clicked()
{
	if(this->ui->labelButton->isChecked())
		setTool("Label");
	else
		setTool("");
}

void MainWindow::viewKeyPress(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key::Key_Escape:
			setTool("");
			break;
	}

}
