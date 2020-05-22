#ifndef LINELENGTHDIMENSION_H
#define LINELENGTHDIMENSION_H

#include "../Label.h"

class LineLengthDimension : public DrawableObject, public UserInputRequester
{
public:
	LineLengthDimension();
	LineLengthDimension(Line *line);

	void resolveTies() override;

	//file handling
	void loadVariables(QString input) override;
	QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> list) override;

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
	double lengthToSet;
	double distanceFromLine = 20;

	int textWidth = 60;
	int textHeight = 20;

	//events
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

	//geometry
	void setGeometryUpdates() override;
	void unsetGeometryUpdates() override;

public slots:

	//user input requests
	void recieveDouble(double value) override;
};

//inline getters and setters
inline void LineLengthDimension::setDistanceFromLine(double distance) { distanceFromLine = distance; }
inline void LineLengthDimension::setLineLength(double length) { lengthToSet = length; }

#endif // LINELENGHTDIMENSION_H
