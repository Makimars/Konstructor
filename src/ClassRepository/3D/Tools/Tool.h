#ifndef TOOL3D_H
#define TOOL3D_H

#include <QObject>

#include "src/ClassRepository/2D/Drawables/Dimensions/CirclesRadiusDifferenceDimension.h"
#include "../Item.h"

namespace Space {

class Tool : public QObject
{
	Q_OBJECT
public:
	virtual void click(QPoint pos) = 0;
	virtual void resetTool() = 0;
};

}

#endif // TOOL3D_H
