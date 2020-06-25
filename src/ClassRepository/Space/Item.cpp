#include "Item.h"

//create coloured triangle
std::vector<Vertex> triangles_vertexes = {
	VERTEX_1, VERTEX_2, VERTEX_TOP,
	VERTEX_2, VERTEX_0, VERTEX_TOP,
	VERTEX_0, VERTEX_1, VERTEX_TOP,
	VERTEX_2, VERTEX_1, VERTEX_0,
};

Item::Item(QVector<DrawableObject*> sketchObjects, QVector3D planePosition, QVector3D planeVector)
{
	this->sketchObjects = sketchObjects;
	this->planePosition = planePosition;
	this->planeVector = planeVector;
	generateVertexes(planePosition, planeVector);
}

void Item::setVectorReference(std::vector<Vertex*> vector, int itemIndex)
{
	vertexes = vector;
	this->itemIndex = itemIndex;

	//copy the original data into the global buffer
	for(int i = 0; i < vertexData.size(); i++)
		*vertexes[i] = vertexData[i];
}

void Item::generateVertexes(QVector3D planePosition, QVector3D planeVector)
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
	vertexData = pointsToSpaceVertexes(planeVertexes, planePosition, planeVector);

	vertexes.reserve(size());
}

void Item::convertToIntermediate(QVector<TransferPoint*> *transferPoints, QVector<TransferLine*> *transferLines)
{
	//generate points
	foreach (DrawableObject *obj, sketchObjects)
	{
		if(obj->getType() == Global::Type_Point)
		{
			Point *point = dynamic_cast<Point*>(obj);

			transferPoints->push_back(TransferPoint::fromDrawablePoint(point));
		}
	}

	//generate lines from Drawable lines
	foreach (DrawableObject *obj, sketchObjects)
	{
		if(obj->getType() == Global::Type_Line)
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
	std::vector<Vertex> planeVertexes;
	foreach(TransferPoint *point, *transferPoints)
	{
		//100 -> from mm to m
		planeVertexes.push_back(Vertex(QVector3D(point->getX()/100,point->getY()/100,0)));
	}

	return planeVertexes;
}

std::vector<Vertex> Item::pointsToSpaceVertexes(std::vector<Vertex> planeVertexes, QVector3D planePosition, QVector3D planeVector)
{
	//generate Space vertexes from points

	//multiply by plane vector

	//add plane base
	foreach(Vertex vertex, planeVertexes)
	{
		QVector3D pos = vertex.position();
		vertex.setPosition(pos + planePosition);
	}

	return planeVertexes;
}
