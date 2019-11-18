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

	this->ui->new_button->setShortcut(Settings::new_file);
	this->ui->open_button->setShortcut(Settings::open_file);
	this->ui->save_button->setShortcut(Settings::save_file);
	this->ui->save_as_button->setShortcut(Settings::save_file_as);
	this->ui->export_button->setShortcut(Settings::export_file);
	this->ui->print_button->setShortcut(Settings::print_file);
	this->ui->settings_button->setShortcut(Settings::open_settings);
	this->ui->quit_button->setShortcut(Settings::quit_app);

	connect(this->ui->main_view_widget, &ViewWidget::keyPressed,
			this, &MainWindow::viewKeyPress
			);
}

MainWindow::~MainWindow()
{
	delete this->ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	this->ui->main_view_widget->update();
}

//----------    Ui handeling    ---------

void MainWindow::setTool(QString tool)
{
	if(tool != "Line")
		this->ui->line_button->setChecked(false);
	if(tool != "Circle")
		this->ui->circle_button->setChecked(false);
	if(tool != "Rectangle")
		this->ui->rectangle_button->setChecked(false);
	if(tool != "Label")
		this->ui->label_button->setChecked(false);

	this->ui->main_view_widget->setTool(tool);
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

void MainWindow::on_new_button_clicked()
{

}

void MainWindow::on_open_button_clicked()
{
	QString file_name = QFileDialog::getOpenFileName(
			this,
                	Global::open_file,
			Settings::user_project_root,
			Global::konstructor_sketch + ";;" + Global::all_files
			);

	QFile file(file_name);
	if(file.exists())
	{
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
			this->ui->main_view_widget->loadFromFile(file.readAll());
	}
}

void MainWindow::on_save_button_clicked()
{
	QString file_name = QFileDialog::getSaveFileName(
			this,
			Global::save_file,
			Settings::user_project_root,
			Global::konstructor_sketch + ";;" + Global::all_files
			);
	this->ui->main_view_widget->saveToFile(file_name);
}

void MainWindow::on_save_as_button_clicked()
{

}

void MainWindow::on_export_button_clicked()
{

}

void MainWindow::on_print_button_clicked()
{

}

void MainWindow::on_settings_button_clicked()
{

}

void MainWindow::on_quit_button_clicked()
{
	this->close();
}

//-----    draw tab    -----

void MainWindow::on_line_button_clicked()
{
	if(this->ui->line_button->isChecked())
		setTool("Line");
	else
		setTool("");
}

void MainWindow::on_circle_button_clicked()
{
	if(this->ui->circle_button->isChecked())
		setTool("Circle");
	else
		setTool("");
}

void MainWindow::on_rectangle_button_clicked()
{
	if(this->ui->rectangle_button->isChecked())
		setTool("Rectangle");
	else
		setTool("");
}

void MainWindow::on_label_button_clicked()
{
	if(this->ui->label_button->isChecked())
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
