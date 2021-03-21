#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDoubleValidator>
#include <QColorDialog>

#include <QDebug>
#include "../Base/Settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SettingsDialog(QWidget *parent = nullptr);
	~SettingsDialog();

	int exec() override;

private slots:
	void on_buttonBox_accepted();

	void on_planeColorButton_clicked();

private:
	Ui::SettingsDialog *ui;
	QDoubleValidator doubleValidator;
	QIntValidator intValidator;

	QColorDialog colorDialog;

	void loadSettings();

	bool colorChanged = false;

signals:
	void saveSettings();
};

#endif // SETTINGSDIALOG_H
