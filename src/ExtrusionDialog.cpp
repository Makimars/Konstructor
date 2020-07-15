#include "ExtrusionDialog.h"
#include "ui_ExtrusionDialog.h"

ExtrusionDialog::ExtrusionDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ExtrusionDialog)
{
	ui->setupUi(this);
}

ExtrusionDialog::~ExtrusionDialog()
{
    delete ui;
}

int ExtrusionDialog::exec(QTreeWidgetItem *treeItem)
{
    if(Item *item = dynamic_cast<Item*>(treeItem))
    {
        referencedItem = item;
        return QDialog::exec();
    }
    return 0;
}
