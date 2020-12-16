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

void WelcomeDialog::on_newProjectButton_clicked()
{
	action = 1;
	close();
}

void WelcomeDialog::on_openProjectButton_clicked()
{
	action = 2;
	close();
}

void WelcomeDialog::on_aboutButton_clicked()
{

}

void WelcomeDialog::on_exitButton_clicked()
{
	close();
}
