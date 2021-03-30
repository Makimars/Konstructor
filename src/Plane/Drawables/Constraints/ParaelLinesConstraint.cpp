#include "ParaelLinesConstraint.h"

ParaelLinesConstraint::ParaelLinesConstraint() : DrawableObject(Global::ParaelLinesConstraint) {}

ParaelLinesConstraint::ParaelLinesConstraint(Line *lineOne, Line *lineTwo) : ParaelLinesConstraint()
{
	this->lines[0] = lineOne;
	this->lines[1] = lineTwo;

	setGeometryUpdates();
	assignPoints();
}

ParaelLinesConstraint::~ParaelLinesConstraint()
{
	follower->removeConstraint();
}

void ParaelLinesConstraint::resolveTies()
{
	QVector2D targetVector = lines[0]->getLineVector();

	QVector2D followerVector(follower->getLocation() - base->getLocation());

	//if targetVector is not in the right direction, reverse
	if(std::abs(
			(targetVector.normalized() - followerVector.normalized()).length()
		) > std::abs(
				(-targetVector.normalized() - followerVector.normalized()).length()
		))
	{
		targetVector = -targetVector;
	}

	follower->setLocation(base->getLocation() + (targetVector.normalized() * lines[1]->getLength()).toPointF());
}

void ParaelLinesConstraint::loadRelations(QVector<DrawableObject *> list)
{
	QStringList varNames = {
		"LineOne",
		"LineTwo"
	};

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->lines[0] = dynamic_cast<Line*>(values[0]);
	this->lines[1] = dynamic_cast<Line*>(values[1]);
	setGeometryUpdates();

	assignPoints();
}

QString ParaelLinesConstraint::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("LineOne", this->lines[0]);
	this->fileAddVar("LineTwo", this->lines[1]);
	return DrawableObject::fileFinish();
}

QRectF ParaelLinesConstraint::boundingRect() const
{
	return QRectF(lines[0]->getCenterPos(), lines[1]->getCenterPos());
}

void ParaelLinesConstraint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(isHidden()) return;

	DrawableObject::paint(painter);

	painter->drawLine(lines[0]->getCenterPos(), lines[1]->getCenterPos());
}

void ParaelLinesConstraint::assignPoints()
{
	if(lines[1]->getStartPoint()->isLocked())
	{
		base = lines[1]->getStartPoint();
		follower = lines[1]->getEndPoint();
	}
	else
	{
		base = lines[1]->getEndPoint();
		follower = lines[1]->getStartPoint();
	}

	follower->addConstraint();
}

void ParaelLinesConstraint::setGeometryUpdates()
{
	this->lines[0]->addGeometryUpdate(this);
	this->lines[1]->addGeometryUpdate(this);
}

void ParaelLinesConstraint::unsetGeometryUpdates()
{
	this->lines[0]->removeGeometryUpdate(this);
	this->lines[1]->removeGeometryUpdate(this);
}
