#ifndef EXTRUSIONDIALOG_H
#define EXTRUSIONDIALOG_H

#include <QDialog>

#include "../Space/Item.h"
#include "../Base/Settings.h"

namespace Ui {
class ExtrusionDialog;
}

struct ExtrusionDialogReturn
{
    double length;
    bool extrusion;
	ExtrusionDirection direction;
};

class ExtrusionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtrusionDialog(QWidget *parent = nullptr);
    ~ExtrusionDialog();

	void show(Item *item);

private:
    Ui::ExtrusionDialog *ui;

    Item *referencedItem;


signals:
	void selectionChanged();

public slots:
	void accept() override;
	void finished(int result);
};

#endif // EXTRUSIONDIALOG_H
