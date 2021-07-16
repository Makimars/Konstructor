#ifndef PARAELLINESCONSTRAINT_H
#define PARAELLINESCONSTRAINT_H

#include "../Drawables.h"

class ParaelLinesConstraint: public DrawableObject
{
public:
	ParaelLinesConstraint();
	ParaelLinesConstraint(Line *lineOne, Line* lineTwo);
	~ParaelLinesConstraint();

	void resolveTies() override;

	void loadRelations(QVector<DrawableObject *> list) override;
	nlohmann::json toJson() override;

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	Line *lines[2];
	Point *base, *follower;

	void assignPoints();

	void setGeometryUpdates() override;
	void unsetGeometryUpdates() override;
};

#endif // PARAELLINESCONSTRAINT_H
