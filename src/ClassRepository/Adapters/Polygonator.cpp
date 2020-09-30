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
	std::vector<QPolygonF> paths;

	//for each point
	while(transferPoints.size() > 0)
	{
		//remove dead ends
		foreach(PointAdapter *point, transferPoints)
			if(point->getNeighborCount() < 2) transferPoints.removeAll(point);
		if(transferPoints.size() < 2) break;

		std::queue<QVector<PointAdapter*>> jobs;

		jobs.push(QVector<PointAdapter*>());
		jobs.front().append(transferPoints.at(0));

		while(jobs.size() > 0)
		{
			QVector<PointAdapter*> workingPath = jobs.front();
			QVector<PointAdapter*> currentNeighbors = workingPath.last()->getNeighborPoints();

			//remove already visited points from neigbor array
			foreach(PointAdapter *visitedPoint, workingPath)
				currentNeighbors.removeAll(visitedPoint);

			//if no neighbors, pop the job, and break loop
			if(currentNeighbors.size() == 0)
			{
				jobs.pop();
				continue;
			}

			//new possible paths
			std::vector<PointAdapter*> newPaths;
			bool finished = false;
			//for each naighbor
			foreach(PointAdapter *neighbor, currentNeighbors)
			{
				//if the neighbor is connected to start point, finish this path
				if(neighbor->isNeighbor(workingPath.at(0)) && workingPath.size() > 1)
				{
					//create a polygon
					QPolygonF finalPolygon;

					workingPath.append(neighbor);

					foreach(PointAdapter *pathPoint, workingPath)
						finalPolygon << pathPoint->toPoint();

					paths.push_back(finalPolygon);

					//remove used points
					std::vector<PointAdapter*> depleted;
					foreach(PointAdapter *pathPoint, workingPath)
					{
						// if the point has only two paths and has been already added to polygon, mark as depleted
						if(pathPoint->getNeighborCount() < 3)
						{
							depleted.push_back(pathPoint);
						}
					}
					foreach(PointAdapter *pathPoint, depleted)
					{
						//remove point from neighbors
						foreach(PointAdapter *p, pathPoint->getNeighborPoints())
							p->removeNeigbor(pathPoint);
						transferPoints.removeAll(pathPoint);
					}

					jobs.empty();
					finished = true;
					break;
				}
				else //add neigbor to list of new paths
				{
					newPaths.push_back(neighbor);
				}
			}

			if(!finished)
			{
				// if at least one new point, add it to current job
				if(newPaths.size() > 0)
					jobs.front().append(newPaths.at(0));
				else
					jobs.pop();

				//for every other point, add it to job queue
				for(int i = 1; i < newPaths.size(); i++)
				{
					QVector<PointAdapter*> newPath = workingPath;
					newPath.append(newPaths.at(i));
					jobs.push(newPath);
				}
			}
		}
	}

	return paths;
}

