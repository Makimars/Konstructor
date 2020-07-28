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

    ExtrusionDialogReturn exec(Item *item);
	std::vector<Polygon*> getSelectedPolygons();

private:
    Ui::ExtrusionDialog *ui;

    Item *referencedItem;

public slots:
	void selectedFaceChanged();
};

#endif // EXTRUSIONDIALOG_H
