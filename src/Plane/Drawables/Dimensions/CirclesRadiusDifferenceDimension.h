#ifndef CIRCLESRADIUSDIFFERENCEDIMENSION_H
#define CIRCLESRADIUSDIFFERENCEDIMENSION_H

#include "../Drawables.h"

class CirclesRadiusDifferenceDimension : public DrawableObject, public UserInputRequester
{
public:
	CirclesRadiusDifferenceDimension();
	CirclesRadiusDifferenceDimension(Circle *circles[2]);

	void resolveTies() override;

	//file handling
	void loadData(nlohmann::json jsonInput) override;
	void loadRelations(QVector<DrawableObject*> list) override;
	nlohmann::json toJson() override;

	//getters and setters
	void setRadiusDifference(double value);
	double getRadiusDifference();

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   ) override;

private:
	double radiusDifference;
	Circle *circles[2];

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

//inline getters and setters
inline void CirclesRadiusDifferenceDimension::setRadiusDifference(double value) { radiusDifference = value; }
inline double CirclesRadiusDifferenceDimension::getRadiusDifference() { return radiusDifference; }

#endif // CIRCLESRADIUSDIFFERENCEDIMENSION_H
