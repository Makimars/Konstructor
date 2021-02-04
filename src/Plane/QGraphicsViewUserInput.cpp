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
	labelProxy = sketchScene->addWidget(&label);
	labelProxy->hide();

	connect(
			&this->inputBox, &QLineEdit::returnPressed,
			this, &QGraphicsViewUserInput::returnInput
			);

	this->doubleValidator.setLocale(Settings::locals);
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
	labelProxy->hide();
	label.setText("");
}

void QGraphicsViewUserInput::setInputBoxLocation(QPointF location)
{
	this->inputBoxProxy->setPos(location);
	labelProxy->setPos(QPoint(location.x(), location.y()-inputBox.height()));
}

void QGraphicsViewUserInput::showInputBox()
{
	this->inputBoxProxy->show();
	labelProxy->show();
	this->inputBox.setFocus();
}

void QGraphicsViewUserInput::requestValue(UserInputRequester *requester)
{
	this->currentReciever = requester;
	showInputBox();
}

//---------     slots     ----------

void QGraphicsViewUserInput::requestDouble(UserInputRequester *requester, QString name)
{
	this->inputBox.setValidator(&this->doubleValidator);
	label.setText(name);
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
