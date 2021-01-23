#include "PointPositionDialog.h"
#include "ui_PointPositionDialog.h"

PointPositionDialog::PointPositionDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PointPositionDialog)
{
	ui->setupUi(this);

	validator.setLocale(Settings::locals);

	this->ui->xEdit->setValidator(&validator);
	this->ui->yEdit->setValidator(&validator);
}

PointPositionDialog::~PointPositionDialog()
{
	delete ui;
}

int PointPositionDialog::exec()
{
	this->ui->xEdit->setText(QString::number(position.x()));
	this->ui->yEdit->setText(QString::number(position.y()));

	return QDialog::exec();
}

void PointPositionDialog::on_buttonBox_accepted()
{
	position.setX(QVariant(this->ui->xEdit->text()).toDouble());
	position.setY(QVariant(this->ui->yEdit->text()).toDouble());

	setResult(DialogCode::Accepted);
	close();
}

void PointPositionDialog::on_buttonBox_rejected()
{
	setResult(DialogCode::Rejected);
	close();
}
