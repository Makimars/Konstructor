#include "Polygonator.h"

Polygonator *Polygonator::instance = nullptr;

Polygonator::Polygonator(){}

Polygonator *Polygonator::getInstance()
{
	if(instance == nullptr) instance = new Polygonator();
	return instance;
}

void Polygonator::recieveDrawing(QVector<DrawableObject*> drawing)
{
	QString sketch;
	for(int i = 0; i < drawing.count(); i++)
	{
		sketch.append(drawing.at(i)->toFileString().toLatin1() + "\n");
	}

	QVector<PointAdapter*> transferPoints;
	QVector<LineAdapter*> transferLines;

	convertToIntermediate(drawing, &transferPoints, &transferLines);
	assignNeigbors(&transferPoints, &transferLines);

	emit sendPolygons(generatePolygons(&transferPoints), sketch);
}

void Polygonator::convertToIntermediate(QVector<DrawableObject*> drawing, QVector<PointAdapter*> *transferPoints, QVector<LineAdapter*> *transferLines)
{
	//convert points
	foreach (DrawableObject *obj, drawing)
	{
		if((obj->getType() == Global::Point) & !obj->isConstructional() & !obj->isHidden())
		{
			Point *point = dynamic_cast<Point*>(obj);

			transferPoints->push_back(PointAdapter::fromDrawablePoint(point));
		}
	}

	//convert lines from Drawable lines
	foreach (DrawableObject *obj, drawing)
	{
		if((obj->getType() == Global::Line) & !obj->isConstructional() & !obj->isHidden())
		{
			Line *line = dynamic_cast<Line*>(obj);

			transferLines->push_back(LineAdapter::fromDrawableLine(line, transferPoints));
		}
	}

	//generate lines from circles, arc
}

void Polygonator::assignNeigbors(QVector<PointAdapter*> *transferPoints, QVector<LineAdapter*> *transferLines)
{
	foreach(PointAdapter *point, *transferPoints)
	{
		foreach(LineAdapter *line, *transferLines)
		{
			if(line->hasPoint(point))
				point->addNeighbor(line->getOtherPoint(point));
		}
	}
}

std::vector<QPolygonF> Polygonator::generatePolygons(QVector<PointAdapter*> *transferPoints)
{
	QPolygonF poly;
	for(int i = 0; i < transferPoints->size(); i++)
	{
		poly.append(transferPoints->at(i)->toPoint());
	}

	std::vector<QPolygonF> polygons;
	polygons.push_back(poly);

	return polygons;
}

