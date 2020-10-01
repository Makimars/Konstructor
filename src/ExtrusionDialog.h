#ifndef EXTRUSIONDIALOG_H
#define EXTRUSIONDIALOG_H

#include <QDialog>
#include "ClassRepository/Space/Item.h"
#include "ClassRepository/Settings.h"

namespace Ui {
class ExtrusionDialog;
}

struct ExtrusionDialogReturn
{
    double length;
    bool extrusion;
    ExtrusionDirection direction;
    int dialogCode;
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
};

#endif // EXTRUSIONDIALOG_H
