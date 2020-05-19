#ifndef TOOL3D_H
#define TOOL3D_H

#include <QObject>

class Tool3D : public QObject
{
	Q_OBJECT
public:
	virtual void click();
	virtual void resetTool();
};

#endif // TOOL3D_H
