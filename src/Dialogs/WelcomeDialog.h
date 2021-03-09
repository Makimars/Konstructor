#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>

#include "AboutDialog.h"
#include "../MainWindow.h"

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit WelcomeDialog(QWidget *parent = nullptr);
	~WelcomeDialog();

	void show(MainWindow *m);

private slots:
	void on_exitButton_clicked();

	void on_newProjectButton_clicked();

	void on_openProjectButton_clicked();

	void on_aboutButton_clicked();

private:
	Ui::WelcomeDialog *ui;

	AboutDialog aboutDialog;
	MainWindow *mainWindow;
};

#endif // WELCOMEDIALOG_H
