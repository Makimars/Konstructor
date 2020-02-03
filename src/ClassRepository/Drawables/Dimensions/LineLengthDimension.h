#ifndef LINELENGTHDIMENSION_H
#define LINELENGTHDIMENSION_H

#include "../../UserInputRequester.h"

#define TYPE_LINE_LENGTH_DIMENSION "LineLengthDimension"

class LineLengthDimension : public DrawableObject, public UserInputRequester
{
public:
	LineLengthDimension(Line *line);

	void resolveTies() override;
	void setValue(double lenght);

	//file handling
	void fromFileString(QString json) override;
	QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> *list) override;

	//getters and setters
	void setDistanceFromLine(double distance);
	void setLineLength(double length);

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

public slots:

	//user input requests
	void recieveDouble(double value) override;

};

#endif // LINELENGHTDIMENSION_H
