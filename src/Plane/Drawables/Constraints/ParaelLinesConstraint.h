#ifndef PARAELLINESCONSTRAINT_H
#define PARAELLINESCONSTRAINT_H

#include "../Drawables.h"

class ParaelLinesConstraint: public DrawableObject
{
public:
	ParaelLinesConstraint();
	ParaelLinesConstraint(Line *lineOne, Line* lineTwo);

public:
	void resolveTies() override;

	void loadVariables(QString input) override;
	void loadRelations(QVector<DrawableObject *> list) override;
	QString toFileString() override;

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	Line *lines[2];

	void setGeometryUpdates() override;
	void unsetGeometryUpdates() override;
};

#endif // PARAELLINESCONSTRAINT_H
