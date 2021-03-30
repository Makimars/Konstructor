#include "LineCenterPointConstraint.h"

LineCenterPointConstraint::LineCenterPointConstraint() : DrawableObject(Global::LineCenterPointConstraint){}

LineCenterPointConstraint::LineCenterPointConstraint(Line *line, Point *point) : LineCenterPointConstraint()
{
	this->line = line;
	centerPoint = point;

	centerPoint->addConstraint();
}

LineCenterPointConstraint::~LineCenterPointConstraint()
{
	centerPoint->removeConstraint();
}

void LineCenterPointConstraint::resolveTies()
{
	QPointF pos = line->getStartPoint()->getLocation();
	QPointF vec = line->getLineVector().toPointF() / 2;

	centerPoint->setLocation(pos + vec);
}

Line *LineCenterPointConstraint::getLine()
{
	return line;
}

Point *LineCenterPointConstraint::getPoint()
{
	return centerPoint;
}

QString LineCenterPointConstraint::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("line", this->line);
	this->fileAddVar("centerPoint", this->centerPoint);
	return DrawableObject::fileFinish();
}

void LineCenterPointConstraint::loadRelations(QVector<DrawableObject *> list)
{
	QStringList varNames = {
		"line",
		"centerPoint"
	};

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->line = dynamic_cast<Line*>(values[0]);
	this->centerPoint = dynamic_cast<Point*>(values[1]);

	setGeometryUpdates();
	centerPoint->addConstraint();
}

QRectF LineCenterPointConstraint::boundingRect() const
{
	QPointF pos = centerPoint->getLocation();

	return QRectF(pos.x()-1, pos.y()-1, 2, 2);
}

QPainterPath LineCenterPointConstraint::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());

	return path;
}

void LineCenterPointConstraint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	resolveTies();
	return;
}

void LineCenterPointConstraint::setGeometryUpdates()
{
	line->addGeometryUpdate(this);
}

void LineCenterPointConstraint::unsetGeometryUpdates()
{
	line->removeGeometryUpdate(this);
}
