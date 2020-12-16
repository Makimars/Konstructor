#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit WelcomeDialog(QWidget *parent = nullptr);
	~WelcomeDialog();

	int action = 0;

private slots:
	void on_exitButton_clicked();

	void on_newProjectButton_clicked();

	void on_openProjectButton_clicked();

	void on_aboutButton_clicked();

private:
	Ui::WelcomeDialog *ui;
};

#endif // WELCOMEDIALOG_H
