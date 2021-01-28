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
	action = WelcomeScreenResult::NewProject;
	close();
}

void WelcomeDialog::on_openProjectButton_clicked()
{
	action = WelcomeScreenResult::OpenProject;
	close();
}

void WelcomeDialog::on_aboutButton_clicked()
{
	action = WelcomeScreenResult::About;
	close();
}

void WelcomeDialog::on_exitButton_clicked()
{
	action = WelcomeScreenResult::Exit;
	close();
}
