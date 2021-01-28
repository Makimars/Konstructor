#ifndef MESSAGESMANAGER_H
#define MESSAGESMANAGER_H

#include "PopUpMessage.h"

class MessagesManager
{
public:
	static void init();

	static int showDialogForm(QString text);
	static int showYesNoForm(QString text);
	static int showOkCancelForm(QString text);

private:
	static PopUpMessage *popUpMessage;

	static void initPopUpMessage(QString text);
};

inline void MessagesManager::initPopUpMessage(QString text)
{
	popUpMessage->clearButtons();
	popUpMessage->setText(text);
}

#endif // MESSAGESMANAGER_H
