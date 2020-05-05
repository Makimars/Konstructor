#ifndef DRAWABLEALREADYRESTRAINEDEXCEPTION_H
#define DRAWABLEALREADYRESTRAINEDEXCEPTION_H

#include <QException>

class DrawableAlreadyRestrainedException : public QException
{
public:
	DrawableAlreadyRestrainedException();
};

#endif // DRAWABLEALREADYRESTRAINEDEXCEPTION_H
