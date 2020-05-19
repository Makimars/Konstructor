#ifndef CIRCLERADIUSDIMENSION_H
#define CIRCLERADIUSDIMENSION_H

#include "LinesAngleDimension.h"

class CircleRadiusDimension : public DrawableObject, public UserInputRequester
{
public:
	CircleRadiusDimension();
	CircleRadiusDimension(Circle *circle);

	void resolveTies() override;

	//file handling
	void loadVariables(QString input) override;
	QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> list) override;

	//getters and setters
	void setRadius(double value);
	double getRadius();

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   ) override;

private:
	double radius;
	Circle *circle;

	int textWidth = 60;
	int textHeight = 20;

	//events
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

	//geometry
	void setGeometryUpdates() override;
	void unsetGeometryUpdates() override;

public slots:

	//user input requests
	void recieveDouble(double value) override;
};

inline void CircleRadiusDimension::setRadius(double value) { radius = value; }
inline double CircleRadiusDimension::getRadius() { return radius; }

#endif // CIRCLERADIUSDIMENSION_H
