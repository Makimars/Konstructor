#include "Polygonator.h"

Polygonator *Polygonator::instance = new Polygonator();

Polygonator::Polygonator(){}

Polygonator *Polygonator::getInstance()
{
	return instance;
}

std::vector<QPolygonF> Polygonator::generatePolygonsFromDrawing(QVector<DrawableObject*> drawing)
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
	reorderPolygons(&polygons);

	//delete adapter poins
	foreach(PointAdapter *point, transferPoints)
		delete point;

	return polygons.toStdVector();
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
		else if (obj->getType() == Global::LineCenterPointConstraint)
		{
			LineCenterPointConstraint *constraint = dynamic_cast<LineCenterPointConstraint*>(obj);

			PointAdapter *centerPoint = pointAdapters.at(originalPoints.indexOf(constraint->getPoint()));
			PointAdapter *linePointOne = pointAdapters.at(originalPoints.indexOf(constraint->getLine()->getStartPoint()));
			PointAdapter *linePointTwo = pointAdapters.at(originalPoints.indexOf(constraint->getLine()->getEndPoint()));

			centerPoint->addNeighbor(linePointOne);
			centerPoint->addNeighbor(linePointTwo);

			linePointOne->addNeighbor(centerPoint);
			linePointTwo->addNeighbor(centerPoint);
		}
		else if (obj->getType() == Global::Arc)
		{
			//Arcs
			Arc *arc = dynamic_cast<Arc*>(obj);
			QVector<PointAdapter*> points;

			double startingAngle, endingAngle;
			PointAdapter *firstEdgePoint;
			PointAdapter *secondEdgePoint;
			{
				QPointF center = arc->getCenter()->getLocation();
				double angleOne = atan2(arc->getFirstEdgePoint()->getLocation().y() - center.y(), arc->getFirstEdgePoint()->getLocation().x() - center.x());
				double angleTwo = atan2(arc->getSecondEdgePoint()->getLocation().y() - center.y(), arc->getSecondEdgePoint()->getLocation().x() - center.x());

				if(angleOne < 0) angleOne = M_PI*2 + angleOne;
				if(angleTwo < 0) angleTwo = M_PI*2 + angleTwo;

				if(angleOne < angleTwo)
				{
					startingAngle = angleOne;
					endingAngle = angleTwo;

					firstEdgePoint = pointAdapters.at(originalPoints.indexOf(arc->getFirstEdgePoint()));
					secondEdgePoint = pointAdapters.at(originalPoints.indexOf(arc->getSecondEdgePoint()));
				}
				else
				{
					startingAngle = angleTwo;
					endingAngle = angleOne;

					firstEdgePoint = pointAdapters.at(originalPoints.indexOf(arc->getSecondEdgePoint()));
					secondEdgePoint = pointAdapters.at(originalPoints.indexOf(arc->getFirstEdgePoint()));
				}
			}


			double radius = arc->getRadius();
			int pointsPerCircle = (int)(radius / Settings::pointPerRadiusCoeficient);
			if(pointsPerCircle < 8) pointsPerCircle = 8;

			double anglePerPoint = (endingAngle - startingAngle) / pointsPerCircle;

			//0 and last point already exists as edge point
			for(int i = 1; i < pointsPerCircle-1; i++)
			{
				double currentAngle = startingAngle + (anglePerPoint*i);

				double x = arc->getCenter()->getX() + (cos(currentAngle) * radius);
				double y = arc->getCenter()->getY() + (sin(currentAngle) * radius);

				PointAdapter *point = new PointAdapter(x, y);
				points.append(point);

				if(i > 1){
					points.at(i-2)->addNeighbor(point);
					point->addNeighbor(points.at(i-2));
				}
			}

			//edge points connections
			firstEdgePoint->addNeighbor(points.at(0));
			points.at(0)->addNeighbor(firstEdgePoint);

			int lastIndex = points.size()-1;
			secondEdgePoint->addNeighbor(points.at(lastIndex));
			points.at(lastIndex)->addNeighbor(secondEdgePoint);

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
		{
			if(point->getNeighborCount() < 2)
			{
				transferPoints.removeAll(point);
				foreach(PointAdapter *secondPoint, transferPoints)
				{
					secondPoint->removeNeigbor(point);
				}
			}
		}
		if(transferPoints.size() < 2) break;

		//finding paths
		std::queue<QVector<PointAdapter*>> jobs;

		jobs.push(QVector<PointAdapter*>());
		jobs.front().append(transferPoints.at(0));

		while(!jobs.empty())
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
			bool jobFinished = false;
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

					jobFinished = true;
					break;
				}
				else //add neigbor to list of new paths
				{
					newPaths.push_back(neighbor);
				}
			}

			if(!jobFinished)
			{
				// if at least one new point, add it to current job
				if(newPaths.size() > 0)
					jobs.front().append(newPaths.at(0));
				else
					jobs.pop();

				//for every other point, add it to job queue
				for(uint32_t i = 1; i < newPaths.size(); i++)
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

void Polygonator::reorderPolygons(QVector<QPolygonF> *polygons)
{
	foreach(QPolygonF polygon, *polygons)
	{
		if(polygon.size() < 3) break;

		//calculate polygon center
		QPointF center;
		for(int i = 0; i < polygon.size(); i++)
		{
			center += polygon.at(i);
		}
		center = center / polygon.size();

		//calculate angle for every point
		QVector<double> angles;
		for(int i = 0; i < polygon.size(); i++)
		{
			double angle = atan2(polygon.at(i).y() - center.y(), polygon.at(i).x() - center.x());
			if(angle < 0)
				angle = M_PI*2 + angle;

			angles.append(angle);
		}

		//if the second point goes in the wrong direction, reverse the order
		if(((angles.at(1) < angles.at(0)) && (*std::max_element(angles.begin(), angles.end()) != angles.at(0))) ||
			((angles.at(1) > angles.at(0)) && (*std::max_element(angles.begin(), angles.end()) == angles.at(0))))
		{
			QPolygonF newPolygon;
			for(int i = 0; i < polygon.size(); i++)
			{
				newPolygon.append(polygon.at(polygon.size() - 1 - i));
			}
			polygon = newPolygon;
		}
	}
}
