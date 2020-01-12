#ifndef USERINPUTREQUESTER_H
#define USERINPUTREQUESTER_H

#include "Drawables/Circle.h"

class UserInputRequester : public QObject
{
    Q_OBJECT
protected:
	UserInputRequester();
        virtual ~UserInputRequester();

signals:
	void requestDouble(UserInputRequester *requester);
	void requestString(UserInputRequester *requeste);

public slots:
	virtual void recieveDouble(double value);
	virtual void recieveString(QString value);

};

#endif // USERINPUTREQUESTER_H
