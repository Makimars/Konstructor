#ifndef LINELENGTHDIMENSION_H
#define LINELENGTHDIMENSION_H

#include "../Label.h"

class LineLengthDimension : public DrawableObject, public UserInputRequester
{
public:
	LineLengthDimension();
	LineLengthDimension(Line *line);
	~LineLengthDimension() override;

	void resolveTies() override;
	void setValue(double lenght);

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
	void setGeometryUpdates();

public slots:

	//user input requests
	void recieveDouble(double value) override;
};

#endif // LINELENGHTDIMENSION_H
