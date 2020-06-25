#include "MainWindow.h"
#include "ui_MainWindow.h"

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

	this->swapMode(Global::Mode::Object);

	connect(this->ui->view2D, &ViewWidget::keyPressed,
			this, &MainWindow::viewKeyPress
			);
	connect(this->ui->view2D, &ViewWidget::showStatusBarMessage,
			this->ui->statusBar, &QStatusBar::showMessage
			);


	connect(this, &MainWindow::setTool,
			this->ui->view2D, &ViewWidget::setTool
			);
	connect(this, &MainWindow::setTool,
			this->ui->view3D, &View3DWidget::setTool
			);
	connect(this, &MainWindow::resetTool,
			this->ui->view2D, &ViewWidget::resetTool
			);

	connect(this, &MainWindow::resetTool,
			this->ui->view3D, &View3DWidget::resetTool
			);


	connect(DrawTool::getInstance(), &DrawTool::requestDrawing,
			this->ui->view2D, &ViewWidget::requestDrawing
			);
	connect(DrawTool::getInstance(), &DrawTool::requestDrawing,
			this, &MainWindow::setDrawing
			);
	connect(this->ui->view2D, &ViewWidget::returnDrawing,
			DrawTool::getInstance(), &DrawTool::recieveDrawing
			);
	connect(this, &MainWindow::finishDrawing,
			this->ui->view2D, &ViewWidget::finishDrawing
			);
}

MainWindow::~MainWindow()
{
	delete this->ui;
}

//----------    Ui handeling    ---------

void MainWindow::refreshTools(int tool)
{
	if(tool != Global::Tools::LineTool)
		this->ui->lineButton->setChecked(false);
	if(tool != Global::Tools::CircleTool)
		this->ui->circleButton->setChecked(false);
	if(tool != Global::Tools::RectangleTool)
		this->ui->rectangleButton->setChecked(false);
	if(tool != Global::Tools::LabelTool)
		this->ui->labelButton->setChecked(false);
	if(tool != Global::Tools::DimensionTool)
		this->ui->dimensionButton->setChecked(false);
	if(tool != Global::Tools::DrawTool)
		this->ui->drawButton->setChecked(false);
	if(tool != Global::Tools::ExtrusionTool)
		this->ui->extrusionButton->setChecked(false);

	emit setTool(tool);
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
		refreshTools(Global::Tools::LineTool);
	else
		refreshTools(Global::Tools::NoTool);
}

void MainWindow::on_circleButton_clicked()
{

	if(this->ui->circleButton->isChecked())
		refreshTools(Global::Tools::CircleTool);
	else
		refreshTools(Global::Tools::NoTool);
}

void MainWindow::on_rectangleButton_clicked()
{
	if(this->ui->rectangleButton->isChecked())
		refreshTools(Global::Tools::RectangleTool);
	else
		refreshTools(Global::Tools::NoTool);
}

void MainWindow::on_labelButton_clicked()
{
	if(this->ui->labelButton->isChecked())
		refreshTools(Global::Tools::LabelTool);
	else
		refreshTools(Global::Tools::NoTool);
}

void MainWindow::on_dimensionButton_clicked()
{
	if(this->ui->dimensionButton->isChecked())
		refreshTools(Global::Tools::DimensionTool);
	else
		refreshTools(Global::Tools::NoTool);
}

void MainWindow::viewKeyPress(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key::Key_Escape:
			refreshTools(Global::Tools::NoTool);
			break;
		case Qt::Key::Key_Enter:
			emit resetTool();
			break;
	}

}

void MainWindow::on_topTabMenu_currentChanged(int index)
{
	swapMode(index);
}

void MainWindow::on_drawButton_clicked()
{
	if(this->ui->drawButton->isChecked())
		refreshTools(Global::Tools::DrawTool);
	else
		refreshTools(Global::Tools::NoTool);
}

void MainWindow::on_finishDrawingButton_clicked()
{
	emit finishDrawing();
	swapMode(Global::Mode::Object);
}

void MainWindow::swapMode(int index)
{
	ui->topTabMenu->setCurrentIndex(index);
	switch (index)
	{
		case Global::Mode::Object:
			ui->view2D->hide();
			ui->view3D->show();
			break;
		case Global::Mode::Draw:
			ui->view2D->show();
			ui->view3D->hide();
			break;
	}
}
