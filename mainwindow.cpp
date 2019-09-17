#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	this->ui->setupUi(this);
	this->timerID = startTimer(50);

	loadSettings();
}

MainWindow::~MainWindow()
{
	delete this->ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    this->ui->main_view_widget->repaint();
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
	QString all_files_filter = tr("All files (*.*)");
	QString konstructor_sketch_filter = tr("Konstructor sketch .kosk (*.kosk");

    QString file_name = QFileDialog::getOpenFileName(
				this,
				tr("Open file"),
                Settings::user_project_root,
				konstructor_sketch_filter + ";;" + all_files_filter
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
    QString konstructor_sketch_filter = tr("Konstructor sketch .kosk (*.kosk");

    QFileDialog save_dialog(
                this,
                tr("Save file"),
                Settings::user_project_root,
                konstructor_sketch_filter
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


