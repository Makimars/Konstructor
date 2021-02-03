#ifndef VIEWWIDGETUSERINPUT_H
#define VIEWWIDGETUSERINPUT_H

#include <QLineEdit>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <QValidator>

#include  "Drawables/Dimensions/LineLengthConstraint.h"

class QGraphicsViewUserInput : public QObject
{
public:
	static void initialize(QGraphicsScene *sketchScene);
	static QGraphicsViewUserInput *getInstance();

	bool isFocused();
	void closeInputBox();
	void setInputBoxLocation(QPointF location);

private:
	static QGraphicsViewUserInput *instance;
	QGraphicsViewUserInput(QGraphicsScene *sketchScene);

	QLineEdit inputBox;
	QGraphicsProxyWidget *inputBoxProxy;
	QLabel label;
	QGraphicsProxyWidget *labelProxy;

	QDoubleValidator doubleValidator;

	UserInputRequester *currentReciever;

	void showInputBox();
	void requestValue(UserInputRequester *requester);
	void returnDouble(double value);
	void returnString(double value);

public slots:
	void requestDouble(UserInputRequester *requester, QString name);
	void requestString(UserInputRequester *requester);

	void returnInput();
};

#endif // VIEWWIDGETUSERINPUT_H
