#ifndef LINESANGLECONSTRAINT_H
#define LINESANGLECONSTRAINT_H

#include "../Drawables.h"

class LinesAngleConstraint : public DrawableObject, public UserInputRequester
{
public:
    LinesAngleConstraint();
	LinesAngleConstraint(Line *lineOne, Line *lineTwo);

    void resolveTies() override;
    //file handling
    void loadVariables(QString input) override;
    QString toFileString() override;
    void loadRelations(QVector<DrawableObject*> list) override;

    //getters and setters
    void setValue(double angle);
    void setDistanceFromCenter(double distance);

    //QGraphicsItem overrides
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget
                       ) override;
public slots:
    void recieveDouble(double value) override;

private:
    double angle;
    double distanceFromCenter;

    Line *lines[2];
    Point *commonPoint;
    Point *edgePoints[2];

    QPointF edgePointsLocations[2];

    int textWidth = 60;
    int textHeight = 20;

    void calculateEdgePoints();

    //events
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    //geometry
    void setGeometryUpdates() override;
    void unsetGeometryUpdates() override;
};

#endif // LINESANGLECONSTRAINT_H
