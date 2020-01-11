#include "QGraphicsViewUserInput.h"

QGraphicsViewUserInput *QGraphicsViewUserInput::instance = nullptr;

QGraphicsViewUserInput *QGraphicsViewUserInput::getInstance(QGraphicsScene *sketchScene)
{
	if(QGraphicsViewUserInput::instance == nullptr)
		QGraphicsViewUserInput::instance = new QGraphicsViewUserInput(sketchScene);

	return QGraphicsViewUserInput::instance;
}

QGraphicsViewUserInput::QGraphicsViewUserInput(QGraphicsScene *sketchScene)
{
	this->inputBoxProxy = sketchScene->addWidget(&this->inputBox);
	this->inputBoxProxy->hide();

	connect(
			&this->inputBox, &QLineEdit::returnPressed,
			this, &QGraphicsViewUserInput::returnInput
			);
}

void QGraphicsViewUserInput::showInputBox()
{
	this->inputBoxProxy->show();
	this->inputBox.setFocus();
}

void QGraphicsViewUserInput::requestValue(UserInputRequester *requester)
{
	this->currentReciever = requester;
	showInputBox();
}

void QGraphicsViewUserInput::requestDouble(UserInputRequester *requester)
{
	this->inputBox.setValidator(&this->doubleValidator);
	requestValue(requester);
}

void QGraphicsViewUserInput::requestString(UserInputRequester *requester)
{
	this->inputBox.setValidator(nullptr);
	requestValue(requester);
}

void QGraphicsViewUserInput::closeInputBox()
{
	this->currentReciever = nullptr;
	this->inputBoxProxy->hide();
}

void QGraphicsViewUserInput::returnInput()
{
	if(this->inputBox.validator() == &this->doubleValidator)
		this->currentReciever->recieveDouble(this->inputBox.text().toDouble());
	else
		this->currentReciever->recieveString(this->inputBox.text());

	closeInputBox();
}
