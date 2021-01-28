#include "Polygonator.h"

Polygonator *Polygonator::instance = new Polygonator();

Polygonator::Polygonator(){}

Polygonator *Polygonator::getInstance()
{
	return instance;
}

std::vector<QPolygonF> Polygonator::generatePolygons(QVector<DrawableObject*> drawing)
{
	if(drawing.size() < 1) return std::vector<QPolygonF>();

	foreach (DrawableObject *obj, drawing)
	{
		if((obj->isConstructional() | obj->isHidden()))
		{
			drawing.removeAll(obj);
		}
	}

	QVector<PointAdapter*> transferPoints = generateAdapters(drawing);
	QVector<QPolygonF> polygons = generatePolygons(transferPoints);

	//delete adapter poins
	foreach(PointAdapter *point, transferPoints)
		delete point;

	return polygons.toStdVector();
}

void Polygonator::recieveDrawing(QVector<DrawableObject*> drawing)
{
	QString sketch;
	for(int i = 0; i < drawing.count(); i++)
	{
		sketch.append(drawing.at(i)->toFileString().toLatin1() + "\n");
	}

	emit sendPolygons(generatePolygons(drawing), sketch);
}

QVector<PointAdapter*> Polygonator::generateAdapters(QVector<DrawableObject*> drawing)
{
	QVector<PointAdapter*> pointAdapters;
	QVector<Point*> originalPoints;

	//generate points
	foreach (DrawableObject *obj, drawing)
	{
		if(obj->getType() == Global::Point)
		{
			Point *point = dynamic_cast<Point*>(obj);

			originalPoints.append(point);
			pointAdapters.append(new PointAdapter(point->getX(), point->getY()));
		}
	}
	//generate connections
	foreach (DrawableObject *obj, drawing)
	{
		if((obj->getType() == Global::Line))
		{
			//Lines

			Line *line = dynamic_cast<Line*>(obj);

			PointAdapter *pointOne = pointAdapters.at(originalPoints.indexOf(line->getStartPoint()));
			PointAdapter *pointTwo = pointAdapters.at(originalPoints.indexOf(line->getEndPoint()));

			pointOne->addNeighbor(pointTwo);
			pointTwo->addNeighbor(pointOne);
		}
		else if(obj->getType() == Global::Circle)
		{
			//Circles

			Circle *circle = dynamic_cast<Circle*>(obj);
			QVector<PointAdapter*> points;

			double radius = circle->getRadius();
			int pointsPerCircle = (int)(radius / Settings::pointPerRadiusCoeficient);
			if(pointsPerCircle < 8) pointsPerCircle = 8;

			double anglePerPoint = M_PI / pointsPerCircle * 2;

			for(int i = 0; i < pointsPerCircle; i++)
			{
				double x = circle->getCenterPoint()->getX() + (cos(i*anglePerPoint) * radius);
				double y = circle->getCenterPoint()->getY() + (sin(i*anglePerPoint) * radius);

				PointAdapter *point = new PointAdapter(x, y);
				points.append(point);

				if(i > 0){
					points.at(i-1)->addNeighbor(point);
					point->addNeighbor(points.at(i-1));
				}
				if(i == pointsPerCircle-1){
					point->addNeighbor(points.at(0));
					points.at(0)->addNeighbor(point);
				}
			}

			pointAdapters.append(points);
		}
	}

	return pointAdapters;
}

QVector<QPolygonF> Polygonator::generatePolygons(QVector<PointAdapter*> transferPoints)
{
	QVector<QPolygonF> paths;

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

					paths.append(finalPolygon);

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

