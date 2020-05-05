#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	this->ui->snappingDistanceLine->setValidator(validator);
	this->ui->pointSizeLine->setValidator(validator);
	this->ui->sketchSizeLine->setValidator(validator);

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

	Settings::sketchSize = QVariant(this->ui->sketchSizeLine->text()).toDouble();

	Settings::angleUnits = (AngleUnits)this->ui->angleUnitBox->currentIndex();

	Settings::newFile = this->ui->newFileEdit->keySequence();
	Settings::openFile = this->ui->openFileEdit->keySequence();
	Settings::saveFile = this->ui->saveFileEdit->keySequence();
	Settings::saveFileAs = this->ui->saveAsFileEdit->keySequence();
	Settings::exportFile = this->ui->exportFileEdit->keySequence();
	Settings::printFile = this->ui->printFileEdit->keySequence();
	Settings::openSettings = this->ui->settingsEdit->keySequence();
	Settings::quitApp = this->ui->quitEdit->keySequence();
}

void SettingsDialog::loadSettings()
{
	this->ui->invertMouseWheel->setChecked(Settings::mouseWheelInvertedZoom);

	this->ui->snappingDistanceLine->setText(QString::number(Settings::snappingDistance));
	this->ui->pointSizeLine->setText(QString::number(Settings::pointMargin.top()));
	this->ui->sketchSizeLine->setText(QString::number(Settings::sketchSize));

	this->ui->angleUnitBox->setCurrentIndex(Settings::angleUnits);

	this->ui->newFileEdit->setKeySequence(Settings::newFile);
	this->ui->openFileEdit->setKeySequence(Settings::openFile);
	this->ui->saveFileEdit->setKeySequence(Settings::saveFile);
	this->ui->saveAsFileEdit->setKeySequence(Settings::saveFileAs);
	this->ui->exportFileEdit->setKeySequence(Settings::exportFile);
	this->ui->printFileEdit->setKeySequence(Settings::printFile);
	this->ui->settingsEdit->setKeySequence(Settings::openSettings);
	this->ui->quitEdit->setKeySequence(Settings::quitApp);
}
