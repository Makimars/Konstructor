#ifndef LINECENTERPOINTCONSTRAINT_H
#define LINECENTERPOINTCONSTRAINT_H

#include "../Drawables.h"

class LineCenterPointConstraint : public DrawableObject
{
public:
	LineCenterPointConstraint();
	LineCenterPointConstraint(Line *line, Point *point);
	~LineCenterPointConstraint();

	void resolveTies() override;

	Line *getLine();
	Point *getPoint();

	//file handling
	void loadRelations(QVector<DrawableObject*> list) override;
	nlohmann::json toJson() override;

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   ) override;

private:
	Line *line;
	Point *centerPoint;

	//geometry
	void setGeometryUpdates() override;
	void unsetGeometryUpdates() override;
};

#endif // LINECENTERPOINTCONSTRAINT_H
