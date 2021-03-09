#include "WelcomeDialog.h"
#include "ui_WelcomeDialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::WelcomeDialog)
{
	ui->setupUi(this);
}

WelcomeDialog::~WelcomeDialog()
{
	delete ui;
}

void WelcomeDialog::show(MainWindow *m)
{
	mainWindow = m;
	QDialog::show();
}

void WelcomeDialog::on_newProjectButton_clicked()
{
	mainWindow->show(Global::Mode::Draw);
	hide();
}

void WelcomeDialog::on_openProjectButton_clicked()
{
	mainWindow->show(Global::Mode::Object);
	hide();
}

void WelcomeDialog::on_aboutButton_clicked()
{
	aboutDialog.exec();
}

void WelcomeDialog::on_exitButton_clicked()
{
	QApplication::quit();
}
