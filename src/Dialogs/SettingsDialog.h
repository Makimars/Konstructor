#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDoubleValidator>

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

private:
	Ui::SettingsDialog *ui;
	QDoubleValidator doubleValidator;
	QIntValidator intValidator;

	void loadSettings();
};

#endif // SETTINGSDIALOG_H
