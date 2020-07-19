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

ExtrusionDialogReturn ExtrusionDialog::exec(Item *item)
{
    referencedItem = item;


    ExtrusionDialogReturn returnData;
    returnData.dialogCode = QDialog::exec();

    ExtrusionDirection direction;
    if(ui->frontButton->isChecked()) direction = ExtrusionDirection::Front;
    if(ui->frontAndBackButton->isChecked()) direction = ExtrusionDirection::FrontAndBack;
    if(ui->backButton->isChecked()) direction = ExtrusionDirection::Back;
    returnData.direction = direction;
    returnData.length = ui->lengthInput->value();
    returnData.extrusion = ui->additiveButton->isChecked();

    return returnData;
}
