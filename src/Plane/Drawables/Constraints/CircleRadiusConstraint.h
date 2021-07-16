#ifndef CIRCLERADIUSCONSTRAINT_H
#define CIRCLERADIUSCONSTRAINT_H

#include "../Drawables.h"

class CircleRadiusConstraint : public DrawableObject, public UserInputRequester
{
public:
	CircleRadiusConstraint();
	CircleRadiusConstraint(Circle *circle);
	~CircleRadiusConstraint();

	void resolveTies() override;

	//file handling
	void loadData(nlohmann::json jsonInput) override;
	void loadRelations(QVector<DrawableObject*> list) override;
	nlohmann::json toJson() override;

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

public slots:
	//user input requests
	void recieveDouble(double value) override;

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

};

inline void CircleRadiusConstraint::setRadius(double value) { radius = value; }
inline double CircleRadiusConstraint::getRadius() { return radius; }

#endif // CIRCLERADIUSCONSTRAINT_H
