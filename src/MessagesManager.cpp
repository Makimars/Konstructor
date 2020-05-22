#include "MessagesManager.h"

PopUpMessage *MessagesManager::popUpMessage = nullptr;

void MessagesManager::init()
{
	MessagesManager::popUpMessage = new PopUpMessage(nullptr);
}

int MessagesManager::showDialogForm(QString text)
{
	initPopUpMessage(text);

	popUpMessage->addButton(QDialogButtonBox::StandardButton::Ok);

	return popUpMessage->exec();
}

int MessagesManager::showYesNoForm(QString text)
{
	initPopUpMessage(text);

	popUpMessage->addButton(QDialogButtonBox::StandardButton::Ok);
	popUpMessage->addButton(QDialogButtonBox::StandardButton::No);

	return popUpMessage->exec();
}

int MessagesManager::showOkCancelForm(QString text)
{
	initPopUpMessage(text);

	popUpMessage->addButton(QDialogButtonBox::StandardButton::Ok);
	popUpMessage->addButton(QDialogButtonBox::StandardButton::Cancel);

	return popUpMessage->exec();
}
