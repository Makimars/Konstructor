#ifndef USERINPUTREQUESTER_H
#define USERINPUTREQUESTER_H

#include <QString>
#include <QObject>

class UserInputRequester : public QObject
{
	Q_OBJECT
protected:
	UserInputRequester(){};
	virtual ~UserInputRequester(){};

signals:
	void requestDouble(UserInputRequester *requester, QString name);
	void requestString(UserInputRequester *requeste, QString name);

public slots:
	virtual void recieveDouble(double value){};
	virtual void recieveString(QString value){};

};

#endif // USERINPUTREQUESTER_H
