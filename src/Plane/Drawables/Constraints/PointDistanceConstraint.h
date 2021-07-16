#ifndef POINTDISTANCECONSTRAINT_H
#define POINTDISTANCECONSTRAINT_H

#include "../Drawables.h"

class PointDistanceConstraint : public DrawableObject, public UserInputRequester
{
public:
	PointDistanceConstraint();
	PointDistanceConstraint(Point *originPoint, Point *drivenPoint);
	~PointDistanceConstraint();

	void resolveTies() override;

	//file handling
	void loadData(nlohmann::json jsonInput) override;
	void loadRelations(QVector<DrawableObject*> list) override;
	nlohmann::json toJson() override;

	//getters and setters
	void setDistanceFromLine(double distance);
	void setLength(double length);

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   ) override;

public slots:

	//user input requests
	void recieveDouble(double value) override;

private:
	Point *originPoint, *drivenPoint;
	double lengthToSet;
	double distanceFromLine = 20;

	int textWidth = 60;
	int textHeight = 20;

	QVector2D getLineVector() const;

	//events
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

	//geometry
	void setGeometryUpdates() override;
	void unsetGeometryUpdates() override;
};

#endif // POINTDISTANCECONSTRAINT_H
