#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>

namespace Ui {
class WelcomeDialog;
}

enum WelcomeScreenResult{
	OpenProject,
	NewProject,
	About,
	Exit
};

class WelcomeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit WelcomeDialog(QWidget *parent = nullptr);
	~WelcomeDialog();

	WelcomeScreenResult action;

private slots:
	void on_exitButton_clicked();

	void on_newProjectButton_clicked();

	void on_openProjectButton_clicked();

	void on_aboutButton_clicked();

private:
	Ui::WelcomeDialog *ui;
};

#endif // WELCOMEDIALOG_H
