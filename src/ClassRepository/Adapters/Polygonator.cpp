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
	if(drawing.size() < 1) return;

	QString sketch;
	for(int i = 0; i < drawing.count(); i++)
	{
		sketch.append(drawing.at(i)->toFileString().toLatin1() + "\n");
	}

	QVector<PointAdapter*> transferPoints;
	QVector<LineAdapter*> transferLines;

	convertToIntermediate(drawing, &transferPoints, &transferLines);
	assignNeigbors(&transferPoints, &transferLines);

	std::vector<QPolygonF> polygons = generatePolygons(transferPoints);

	//delete transfer poins and lines
	foreach(PointAdapter *point, transferPoints)
		delete point;
	foreach(LineAdapter *line, transferLines)
		delete line;

	emit sendPolygons(polygons, sketch);
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

std::vector<QPolygonF> Polygonator::generatePolygons(QVector<PointAdapter*> transferPoints)
{
	//remove edge points
	foreach(PointAdapter *point, transferPoints)
	{
		if(point->getNeighborCount() < 2) transferPoints.removeAll(point);
	}

	std::vector<QPolygonF> paths;

	//for each point
	//for(int i = 0; i < transferPoints.size(); i++)
	while(transferPoints.size() > 0)
	{
		foreach(PointAdapter *point, transferPoints)
			point->setVisited(false);

		std::queue<QVector<PointAdapter*>> jobs;

		jobs.push(QVector<PointAdapter*>());
		jobs.front().append(transferPoints.at(0));

		while(jobs.size() > 0)
		{
			QVector<PointAdapter*> workingPath = jobs.front();

			QVector<PointAdapter*> currentNeighbors = workingPath.last()->getNeighborPoints();
			if(workingPath.size() > 1)
				currentNeighbors.removeAll(workingPath.at(workingPath.size()-2));

			std::vector<PointAdapter*> newPaths;
			foreach(PointAdapter *neighbor, currentNeighbors)
			{
				//if this point is not in the path already
				if(workingPath.lastIndexOf(neighbor) == -1 & transferPoints.size() > 0)
				{
					//finishes this path
					if(neighbor->isNeighbor(transferPoints.at(0)) && workingPath.size() > 1)
					{
						//create a polygon
						QPolygonF finalPolygon;

						foreach(PointAdapter *pathPoint, workingPath)
							finalPolygon << pathPoint->toPoint();
						finalPolygon << neighbor->toPoint();

						paths.push_back(finalPolygon);

						//remove used points
						std::vector<PointAdapter*> depleted;
						foreach(PointAdapter *pathPoint, workingPath)
						{
							// if the point has only two paths and has been already added to polygon, mark as depleted
							if(pathPoint->getNeighborCount() < 3)
								depleted.push_back(pathPoint);
						}
						if(neighbor->getNeighborCount() < 3)
							depleted.push_back(neighbor);

						foreach(PointAdapter *pathPoint, depleted)
							transferPoints.removeAll(pathPoint);
					}
					else
					{
						newPaths.push_back(neighbor);
					}
				}
			}
			foreach(PointAdapter *newPoint, newPaths)
			{
				QVector<PointAdapter*> newPath = workingPath;
				newPath.append(newPoint);
				jobs.push(newPath);
			}

			//close this job
			jobs.pop();
		}
	}

	return paths;
}

