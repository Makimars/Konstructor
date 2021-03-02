#ifndef LINESANGLEDIMENSION_H
#define LINESANGLEDIMENSION_H

#include "../Drawables.h"

class LinesAngleDimension : public DrawableObject, public UserInputRequester
{
public:
	LinesAngleDimension();
	LinesAngleDimension(Line *lines[2]);

	void resolveTies() override;
	//file handling
	void loadVariables(QString input) override;
	QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> list) override;

	//getters and setters
	void setValue(double angle);
	void setDistanceFromCenter(double distance);

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   ) override;

private:
	double angle;
	double distanceFromCenter;

	Line *lines[2];
	Point *commonPoint;
	Point *edgePoints[2];

	int textWidth = 60;
	int textHeight = 20;

	void calculateEdgePoints();

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
inline void LinesAngleDimension::setValue(double angle) { this->angle = angle; }
inline void LinesAngleDimension::setDistanceFromCenter(double distance) { distanceFromCenter = distance; }

#endif // LINESANGLEDIMENSION_H
