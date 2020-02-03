#ifndef LINESANGLEDIMENSION_H
#define LINESANGLEDIMENSION_H

#include "LineLengthDimension.h"

#define TYPE_LINE_ANGLE_DIMENSION "LineAngleDimension"

class LinesAngleDimension : public DrawableObject, public UserInputRequester
{
public:
	LinesAngleDimension(Line *lines[2], double angle);

	void resolveTies() override;
	void setValue(double angle);

	//file handling
	void fromFileString(QString json) override;
	QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> *list) override;

	//getters and setters
	void setDistanceFromCenter(float distance);

	//user input requests
	void recieveDouble(double value) override;

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   ) override;

private:
	double angle;
	float distanceFromCenter;

	Line *lines[2];
	Point *commonPoint;
	Point *edgePoints[2];

	int textWidth = 60;
	int textHeight = 20;

	//events
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // LINESANGLEDIMENSION_H
