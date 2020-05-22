#include "PopUpMessage.h"
#include "ui_PopUpMessage.h"

PopUpMessage::PopUpMessage(QWidget *parent) :
	ui(new Ui::PopUpMessage)
{
        ui->setupUi(this);
}

PopUpMessage::~PopUpMessage()
{
    delete ui;
}

void PopUpMessage::addButton(QDialogButtonBox::StandardButton button)
{
    ui->buttonBox->addButton(button);
}

void PopUpMessage::clearButtons()
{
    ui->buttonBox->clear();
}

void PopUpMessage::setText(QString text)
{
    ui->label->setText(text);
}
