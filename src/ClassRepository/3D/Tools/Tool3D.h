#ifndef TOOL3D_H
#define TOOL3D_H

#include <QObject>

#include "src/ClassRepository/2D/Drawables/Dimensions/CirclesRadiusDifferenceDimension.h"
#include "../Item.h"

class Tool3D : public QObject
{
	Q_OBJECT
public:
	virtual void click(QPoint pos){}
	virtual void resetTool(){}
};


#endif // TOOL3D_H
