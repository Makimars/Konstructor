#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	this->ui->snappingDistanceLine->setValidator(&doubleValidator);
	this->ui->pointSizeLine->setValidator(&doubleValidator);
	this->ui->sketchSizeLine->setValidator(&doubleValidator);
	this->ui->lineShapeSizeLine->setValidator(&intValidator);

	this->ui->angleUnitBox->addItem(tr("Degrees"));
	this->ui->angleUnitBox->addItem(tr("Radians"));
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

int SettingsDialog::exec()
{
	QDialog::exec();
	loadSettings();
}

void SettingsDialog::on_buttonBox_accepted()
{
	Settings::mouseWheelInvertedZoom = this->ui->invertMouseWheel->isChecked();

	Settings::snappingDistance = QVariant(this->ui->snappingDistanceLine->text()).toDouble();

	double pointSize =  QVariant(this->ui->pointSizeLine->text()).toDouble();
	Settings::pointMargin = QMargins(pointSize,pointSize,pointSize,pointSize);
	Settings::lineShapeSize = QVariant(this->ui->lineShapeSizeLine->text()).toDouble();

	Settings::sketchSize = QVariant(this->ui->sketchSizeLine->text()).toDouble();

	Settings::angleUnits = (AngleUnits)this->ui->angleUnitBox->currentIndex();

	Settings::newFile = this->ui->newFileEdit->keySequence();
	Settings::openFile = this->ui->openFileEdit->keySequence();
	Settings::saveFile = this->ui->saveFileEdit->keySequence();
	Settings::exportFile = this->ui->exportFileEdit->keySequence();
	Settings::openSettings = this->ui->settingsEdit->keySequence();
}

void SettingsDialog::loadSettings()
{
	this->ui->invertMouseWheel->setChecked(Settings::mouseWheelInvertedZoom);

	this->ui->snappingDistanceLine->setText(QString::number(Settings::snappingDistance));
	this->ui->pointSizeLine->setText(QString::number(Settings::pointMargin.top()));
	this->ui->sketchSizeLine->setText(QString::number(Settings::sketchSize));
	this->ui->lineShapeSizeLine->setText(QString::number(Settings::lineShapeSize));

	this->ui->angleUnitBox->setCurrentIndex(Settings::angleUnits);

	this->ui->newFileEdit->setKeySequence(Settings::newFile);
	this->ui->openFileEdit->setKeySequence(Settings::openFile);
	this->ui->saveFileEdit->setKeySequence(Settings::saveFile);
	this->ui->exportFileEdit->setKeySequence(Settings::exportFile);
	this->ui->settingsEdit->setKeySequence(Settings::openSettings);
}
