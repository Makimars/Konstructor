#include "PointDistanceConstraint.h"

PointDistanceConstraint::PointDistanceConstraint() : DrawableObject(Global::Types::PointDistanceConstraint){}

PointDistanceConstraint::PointDistanceConstraint(Point *originPoint, Point *drivenPoint) : PointDistanceConstraint()
{
	this->originPoint = originPoint;
	this->drivenPoint = drivenPoint;
	setGeometryUpdates();

	lengthToSet = drivenPoint->distanceFrom(originPoint->getLocation());
	originPoint->addConstraint();
}

PointDistanceConstraint::~PointDistanceConstraint()
{
	originPoint->removeConstraint();
}

void PointDistanceConstraint::resolveTies()
{
	drivenPoint->setLocation(
				originPoint->getLocation() + (getLineVector().normalized() * lengthToSet).toPointF()
			);
}

void PointDistanceConstraint::loadVariables(QString input)
{
	QStringList varNames = {
		"lengthToSet",
		"distanceFromLine"
	};

	QVector<QVariant> variables = fetchVariables(input, varNames);

	this->lengthToSet = variables[0].toDouble();
	this->distanceFromLine = variables[1].toDouble();
}

QString PointDistanceConstraint::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("lengthToSet", this->lengthToSet);
	this->fileAddVar("distanceFromLine", this->distanceFromLine);
	this->fileAddVar("originPoint", this->originPoint->getId());
	this->fileAddVar("drivenPoint", this->drivenPoint->getId());
	return this->fileFinish();
}

void PointDistanceConstraint::loadRelations(QVector<DrawableObject *> list)
{
	QStringList varNames = {
		"originPoint",
		"drivenPoint"
	};

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->originPoint = dynamic_cast<Point*>(values[0]);
	this->drivenPoint = dynamic_cast<Point*>(values[1]);
	setGeometryUpdates();
	originPoint->addConstraint();
}

void PointDistanceConstraint::setDistanceFromLine(double distance)
{
	this->distanceFromLine = distance;
}

void PointDistanceConstraint::setLength(double length)
{
	lengthToSet = length;
}

QRectF PointDistanceConstraint::boundingRect() const
{
	std::vector<QPointF> points = shape().toFillPolygon().toStdVector();
	std::vector<double> x, y;
	for(uint32_t i = 0; i < points.size(); i++)
	{
		x.push_back(points.at(i).x());
		y.push_back(points.at(i).y());
	}

	return QRectF(
		QPointF(
			*std::min_element(x.begin(), x.end()),
			*std::min_element(y.begin(), y.end())
			),
		QPointF(
			*std::max_element(x.begin(), x.end()),
			*std::max_element(y.begin(), y.end())
			)
	);
}

QPainterPath PointDistanceConstraint::shape() const
{
	QPolygonF polygon;

	QVector2D lineVector = getLineVector();
	//get normal vector of line with defined length
	QVector2D normalVector(
				-lineVector.y(),
				lineVector.x()
						);

	normalVector.normalize();
	if(distanceFromLine < 0)
		normalVector *= this->distanceFromLine - textHeight;
	else
		normalVector *= this->distanceFromLine + textHeight;

	//edge points
	QPointF aboveStartPoint(
				originPoint->getX() + normalVector.x(),
				originPoint->getY() + normalVector.y()
				);
	QPointF aboveEndPoint(
				drivenPoint->getX() + normalVector.x(),
				drivenPoint->getY() + normalVector.y()
				);

	normalVector.normalize();
	normalVector *= -textHeight;

	QPointF startPointTwo(
				aboveStartPoint.x() + normalVector.x(),
				aboveStartPoint.y() + normalVector.y()
				);
	QPointF endPointTwo(
				aboveEndPoint.x() + normalVector.x(),
				aboveEndPoint.y() + normalVector.y()
				);

	polygon << aboveStartPoint
			<< aboveEndPoint
			<< endPointTwo
			<< startPointTwo;

	QPainterPath path;
	path.setFillRule(Qt::FillRule::OddEvenFill);
	path.addPolygon(polygon);

	return path;
}

void PointDistanceConstraint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(this->isHidden())return;

	DrawableObject::paint(painter);

	QVector2D lineVector = getLineVector().normalized();

	//get normal vector of line with defined length
	QVector2D normalVector(
				-lineVector.y(),
				lineVector.x()
						);
	normalVector *= this->distanceFromLine;

	//edge points
	QPointF aboveStartPoint(
				originPoint->getX() + normalVector.x(),
				originPoint->getY() + normalVector.y()
				);
	QPointF aboveEndPoint(
				drivenPoint->getX() + normalVector.x(),
				drivenPoint->getY() + normalVector.y()
				);

	//drawing the lines
	painter->drawLine(originPoint->getLocation(), aboveStartPoint);
	painter->drawLine(aboveStartPoint, aboveEndPoint);
	painter->drawLine(aboveEndPoint, drivenPoint->getLocation());

	//drawing text

	double textAngle = qRadiansToDegrees(
				qAtan(normalVector.x() / normalVector.y())
										);

	QPointF	centerPoint(
				(aboveStartPoint.x() + aboveEndPoint.x()) / 2,
				(aboveStartPoint.y() + aboveEndPoint.y()) / 2
				);

	painter->save();
	painter->translate(centerPoint);
	//rotate funciton rotates clockwise, we have angle counter clockwise
	painter->rotate(-textAngle);
	if(lineVector.x() < 0 & this->distanceFromLine < 0	||
		lineVector.x() > 0 & this->distanceFromLine > 0	)
		painter->translate(QPointF(0,textHeight));

	QRectF textRect(-this->textWidth / 2, -this->textHeight, this->textWidth, this->textHeight);
	painter->drawText(textRect, QString::number(this->lengthToSet));

	painter->resetTransform();
	painter->restore();
}

void PointDistanceConstraint::recieveDouble(double value)
{
	this->lengthToSet = value;
	resolveTies();
}

QVector2D PointDistanceConstraint::getLineVector() const
{
	return QVector2D(drivenPoint->getLocation() - originPoint->getLocation());
}

void PointDistanceConstraint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->isDraging())
	{
		this->distanceFromLine = -Line::signedDistanceFrom(originPoint->getLocation(), drivenPoint->getLocation(), event->pos());
	}
}

void PointDistanceConstraint::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	emit requestDouble(this, "Length");
}

void PointDistanceConstraint::setGeometryUpdates()
{
	originPoint->addGeometryUpdate(this);
	drivenPoint->addGeometryUpdate(this);
}

void PointDistanceConstraint::unsetGeometryUpdates()
{
	originPoint->removeGeometryUpdate(this);
	drivenPoint->removeGeometryUpdate(this);
}
