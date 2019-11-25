#ifndef LINELENGTHDIMENSION_H
#define LINELENGTHDIMENSION_H

#include "../Circle.h"

class LineLengthDimension : public DrawableObject
{
public:
		LineLengthDimension(Line *line, double lenght);

	void resolveTies() override;
	void setValue(double lenght);

	//file handling
	void fromFileString(QString json) override;
	QString toFileString() override;
	void loadRelations(QVector<DrawableObject*> *list) override;
	void setDistanceFromLine(double distance);

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   ) override;

private:
	Line *attachedLine;
	double lengthToSet, distanceFromLine;

};

#endif // LINELENGHTDIMENSION_H
