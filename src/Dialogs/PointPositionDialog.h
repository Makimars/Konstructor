#ifndef POINTPOSITIONDIALOG_H
#define POINTPOSITIONDIALOG_H

#include <QDialog>
#include <QDoubleValidator>

#include "../Base/Settings.h"

namespace Ui {
class PointPositionDialog;
}

class PointPositionDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PointPositionDialog(QWidget *parent = nullptr);
	~PointPositionDialog();

	int exec() override;

	QPointF position;

private slots:
	void on_buttonBox_accepted();

	void on_buttonBox_rejected();

private:
	Ui::PointPositionDialog *ui;
	QDoubleValidator validator;
};

#endif // POINTPOSITIONDIALOG_H
