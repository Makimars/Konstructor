#include "Item.h"

Item::Item(QVector<DrawableObject*> sketchObjects, Space::Plane *plane)
{
	this->sketchObjects = sketchObjects;
	this->basePlane = plane;
	generateVertexes(plane);

	plane->addChild(this);
}

void Item::setVectorReference(std::vector<Vertex*> vector, int itemIndex)
{
	vertexes = vector;
	this->itemIndex = itemIndex;

	//copy the original data into the global buffer
	for(int i = 0; i < vertexData.size(); i++)
		*vertexes[i] = vertexData[i];
}

void Item::generateVertexes(Space::Plane *plane)
{
	vertexData.clear();
	QVector<TransferPoint*> transferPoints;
	QVector<TransferLine*> transferLines;

	//convert to intermediate lines and points
	convertToIntermediate(&transferPoints, &transferLines);

	//assign neigbors to points
	assignNeigbors(&transferPoints, &transferLines);

	//convert to triangles
	std::vector<Vertex> planeVertexes = triangularize(&transferPoints);

	//convert to space
	vertexData = pointsToSpaceVertexes(planeVertexes, plane);

	vertexes.reserve(size());
}

void Item::convertToIntermediate(QVector<TransferPoint*> *transferPoints, QVector<TransferLine*> *transferLines)
{
	//generate points
	foreach (DrawableObject *obj, sketchObjects)
	{
		if(obj->getType() == Global::Point)
		{
			Point *point = dynamic_cast<Point*>(obj);

			transferPoints->push_back(TransferPoint::fromDrawablePoint(point));
		}
	}

	//generate lines from Drawable lines
	foreach (DrawableObject *obj, sketchObjects)
	{
		if(obj->getType() == Global::Line)
		{
			Line *line = dynamic_cast<Line*>(obj);

			transferLines->push_back(TransferLine::fromDrawableLine(line, transferPoints));
		}
	}

	//generate lines from circles
}

void Item::assignNeigbors(QVector<TransferPoint*> *transferPoints, QVector<TransferLine*> *transferLines)
{
	//for every transfer point link neighboring points from lines
	foreach(TransferPoint *point, *transferPoints)
	{
		foreach(TransferLine *line, *transferLines)
		{
			if(line->hasPoint(point))
				point->addNeighbor(line->getOtherPoint(point));
		}
	}
}

std::vector<Vertex> Item::triangularize(QVector<TransferPoint*> *transferPoints)
{
	//generate vertexData from transferPoints

	std::vector<double> coordinates;
	for(int i = 0; i < transferPoints->count(); i++)
	{
		coordinates.push_back(transferPoints->at(i)->getX());
		coordinates.push_back(transferPoints->at(i)->getY());
	}

	delaunator::Delaunator d(coordinates);
	std::vector<Vertex> planeVertexes;

	for(std::size_t i = 0; i < d.triangles.size(); i+=3)
	{
		planeVertexes.push_back(Vertex(QVector3D(
			d.coords[2 * d.triangles[i]],
			d.coords[2 * d.triangles[i] + 1],
			0
		)));
		planeVertexes.push_back(Vertex(QVector3D(
			d.coords[2 * d.triangles[i + 1]],
			d.coords[2 * d.triangles[i + 1] + 1],
			0
		)));
		planeVertexes.push_back(Vertex(QVector3D(
			d.coords[2 * d.triangles[i + 2]],
			d.coords[2 * d.triangles[i + 2] + 1],
			0
		)));
	}

	for(int i = 0; i < planeVertexes.size(); i++)
	{
		planeVertexes.at(i).setColor(QVector3D(0.1, 0.1, 0.1));
	}

	return planeVertexes;
}

std::vector<Vertex> Item::pointsToSpaceVertexes(std::vector<Vertex> planeVertexes, Space::Plane *plane)
{
	//generate Space vertexes from points

	//multiply by plane vector

	//add plane base
	foreach(Vertex vertex, planeVertexes)
	{
		QVector3D pos = vertex.position();
		vertex.setPosition(pos + plane->getPosition());
	}

	return planeVertexes;
}
