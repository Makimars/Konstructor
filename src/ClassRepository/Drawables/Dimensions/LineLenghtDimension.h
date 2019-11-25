#ifndef LINELENGHTDIMENSION_H
#define LINELENGHTDIMENSION_H

#include "../Circle.h"

class LineLenghtDimension : public DrawableObject
{
public:
	LineLenghtDimension(Line *line, double lenght);

	void resolveTies() override;
	void setValue(double lenght);

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
