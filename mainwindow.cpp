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

	connect(this->ui->main_view_widget, &ViewWidget::escape,
			this, &MainWindow::escape
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
	QFileDialog save_dialog(
				this,
                Global::save_file,
				Settings::user_project_root,
                Global::konstructor_sketch
				);
	save_dialog.setDefaultSuffix("kosk");
	save_dialog.exec();

	if(save_dialog.selectedFiles().length() > 0)
		this->ui->main_view_widget->saveToFile(save_dialog.selectedFiles().first());

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

void MainWindow::escape()
{
	setTool("");
}
