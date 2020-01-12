#ifndef LINELENGTHDIMENSION_H
#define LINELENGTHDIMENSION_H

#include "../../UserInputRequester.h"

#define TYPE_LINE_LENGTH_DIMENSION "LineLengthDimension"

class LineLengthDimension : public DrawableObject, public UserInputRequester
{
public:
	LineLengthDimension(Line *line, double lenght);

	void resolveTies() override;
	void setValue(double lenght);

	//file handling
	void fromFileString(QString json) override;
	QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> *list) override;
	void setDistanceFromLine(double distance);

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
	Line *attachedLine;
	double lengthToSet, distanceFromLine;

	int textWidth = 60;
	int textHeight = 20;

	//events
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

};

#endif // LINELENGHTDIMENSION_H
