#ifndef LOCKPOSITIONTOOL_H
#define LOCKPOSITIONTOOL_H

#include "Tool.h"

class LockPositionTool : public Tool
{
public:
    static LockPositionTool *getInstance();

    void click(DrawableObject *clickedObject, QPointF pos) override;
    void resetTool() override;

public slots:
    void mouseMoveEvent(QPointF pos) override;

private:
    LockPositionTool();
    static LockPositionTool *instance;
};

#endif // LOCKPOSITIONTOOL_H
