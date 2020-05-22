#ifndef POPUPMESSAGE_H
#define POPUPMESSAGE_H

#include <QDialog>
#include <QDialogButtonBox>

namespace Ui {
class PopUpMessage;
}

class PopUpMessage : public QDialog
{
    Q_OBJECT

public:
    explicit PopUpMessage(QWidget *parent = nullptr);
    ~PopUpMessage();

    void addButton(QDialogButtonBox::StandardButton button);
    void clearButtons();
    void setText(QString text);


private:
    Ui::PopUpMessage *ui;
};

#endif // POPUPMESSAGE_H
