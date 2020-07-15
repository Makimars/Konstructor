#ifndef EXTRUSIONDIALOG_H
#define EXTRUSIONDIALOG_H

#include <QDialog>
#include "ClassRepository/Space/Item.h"

namespace Ui {
class ExtrusionDialog;
}

class ExtrusionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtrusionDialog(QWidget *parent = nullptr);
    ~ExtrusionDialog();

    int exec(QTreeWidgetItem *treeItem);

private:
    Ui::ExtrusionDialog *ui;

    Item *referencedItem;
};

#endif // EXTRUSIONDIALOG_H
