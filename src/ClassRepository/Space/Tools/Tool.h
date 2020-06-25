#ifndef SPACETOOL_H
#define SPACETOOL_H

#include <QObject>

#include "src/ClassRepository/Plane/Drawables/Dimensions/CirclesRadiusDifferenceDimension.h"
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

#endif // SPACETOOL_H
