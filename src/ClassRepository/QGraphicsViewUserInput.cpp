#include "QGraphicsViewUserInput.h"

QGraphicsViewUserInput *QGraphicsViewUserInput::instance = nullptr;

void QGraphicsViewUserInput::initialize(QGraphicsScene *sketchScene)
{
	if(QGraphicsViewUserInput::instance == nullptr)
		QGraphicsViewUserInput::instance = new QGraphicsViewUserInput(sketchScene);
}

QGraphicsViewUserInput *QGraphicsViewUserInput::getInstance()
{
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

bool QGraphicsViewUserInput::isFocused()
{
	return this->inputBox.hasFocus();
}

void QGraphicsViewUserInput::closeInputBox()
{
	this->currentReciever = nullptr;
	this->inputBoxProxy->hide();
	this->inputBox.setText("");
}

void QGraphicsViewUserInput::setInputBoxLocation(QPointF location)
{
	this->inputBoxProxy->setPos(location);
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

//---------     slots     ----------

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

void QGraphicsViewUserInput::returnInput()
{
	if(this->inputBox.validator() == &this->doubleValidator)
		this->currentReciever->recieveDouble(this->inputBox.text().toDouble());
	else
		this->currentReciever->recieveString(this->inputBox.text());

	closeInputBox();
}
