#ifndef LINELENGTHCONSTRAINT_H
#define LINELENGTHCONSTRAINT_H

#include "CirclesRadiusDifferenceDimension.h"

class LineLengthConstraint : public DrawableObject, public UserInputRequester
{
public:
	LineLengthConstraint();
	LineLengthConstraint(Point *originPoint, Point *drivenPoint);

	void resolveTies() override;

	//file handling
	void loadVariables(QString input) override;
	QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> list) override;

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

#endif // LINELENGTHCONSTRAINT_H
