#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	this->ui->pointSizeLine->setValidator(&doubleValidator);
	this->ui->sketchSizeLine->setValidator(&doubleValidator);
	this->ui->lineShapeSizeLine->setValidator(&intValidator);
	this->ui->maxItemSidePlanes->setValidator(&intValidator);

	this->ui->angleUnitBox->addItem(tr("Radians"));
	this->ui->angleUnitBox->addItem(tr("Degrees"));
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

int SettingsDialog::exec()
{
	loadSettings();
	QDialog::exec();
	return 0;
}

void SettingsDialog::on_buttonBox_accepted()
{
	Settings::mouseWheelInvertedZoom = this->ui->invertMouseWheel->isChecked();
	Settings::gridSnapping = this->ui->gridSnapping->isChecked();

	double pointSize =  QVariant(this->ui->pointSizeLine->text()).toDouble();
	Settings::pointMargin = QMargins(pointSize,pointSize,pointSize,pointSize);
	Settings::lineShapeSize = QVariant(this->ui->lineShapeSizeLine->text()).toDouble();

	Settings::sketchSize = QVariant(this->ui->sketchSizeLine->text()).toDouble();
	Settings::maxItemSidePlanes = QVariant(this->ui->maxItemSidePlanes->text()).toUInt();

	Settings::angleUnits = (AngleUnits)this->ui->angleUnitBox->currentIndex();

	Settings::newFile = this->ui->newFileEdit->keySequence();
	Settings::openFile = this->ui->openFileEdit->keySequence();
	Settings::saveFile = this->ui->saveFileEdit->keySequence();
	Settings::exportFile = this->ui->exportFileEdit->keySequence();
	Settings::openSettings = this->ui->settingsEdit->keySequence();

	QColor color = colorDialog.selectedColor();
	Settings::planeColor = QVector4D(color.redF(), color.greenF(), color.blueF(), 0.3);
}

void SettingsDialog::loadSettings()
{
	this->ui->invertMouseWheel->setChecked(Settings::mouseWheelInvertedZoom);
	this->ui->gridSnapping->setChecked(Settings::gridSnapping);

	this->ui->pointSizeLine->setText(QString::number(Settings::pointMargin.top()));
	this->ui->sketchSizeLine->setText(QString::number(Settings::sketchSize));
	this->ui->lineShapeSizeLine->setText(QString::number(Settings::lineShapeSize));
	this->ui->maxItemSidePlanes->setText(QString::number(Settings::maxItemSidePlanes));

	this->ui->angleUnitBox->setCurrentIndex(Settings::angleUnits);

	this->ui->newFileEdit->setKeySequence(Settings::newFile);
	this->ui->openFileEdit->setKeySequence(Settings::openFile);
	this->ui->saveFileEdit->setKeySequence(Settings::saveFile);
	this->ui->exportFileEdit->setKeySequence(Settings::exportFile);
	this->ui->settingsEdit->setKeySequence(Settings::openSettings);


	QColor color(Settings::planeColor.x(), Settings::planeColor.y(), Settings::planeColor.z(), Settings::planeColor.w());
	colorDialog.setCustomColor(0, color);
	colorDialog.setCurrentColor(color);
}

void SettingsDialog::on_planeColorButton_clicked()
{
	colorDialog.exec();
}
