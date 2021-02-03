#include "LineLengthConstraint.h"

LineLengthConstraint::LineLengthConstraint() : DrawableObject(Global::Types::LineLengthConstraint){}

LineLengthConstraint::LineLengthConstraint(Point *originPoint, Point *drivenPoint) : LineLengthConstraint()
{
	this->originPoint = originPoint;
	this->drivenPoint = drivenPoint;
	setGeometryUpdates();

	lengthToSet = drivenPoint->distanceFrom(originPoint->getLocation());
	originPoint->addConstrant();
}

LineLengthConstraint::~LineLengthConstraint()
{
	originPoint->removeConstraint();
}

void LineLengthConstraint::resolveTies()
{
	drivenPoint->setLocation(
				originPoint->getLocation() + (getLineVector().normalized() * lengthToSet).toPointF()
			);
}

void LineLengthConstraint::loadVariables(QString input)
{
	QStringList varNames = {
		"lengthToSet",
		"distanceFromLine"
	};

	QVector<QVariant> variables = fetchVariables(input, varNames);

	this->lengthToSet = variables[0].toDouble();
	this->distanceFromLine = variables[1].toDouble();
}

QString LineLengthConstraint::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("lengthToSet", this->lengthToSet);
	this->fileAddVar("distanceFromLine", this->distanceFromLine);
	this->fileAddVar("originPoint", this->originPoint->getId());
	this->fileAddVar("drivenPoint", this->drivenPoint->getId());
	return this->fileFinish();
}

void LineLengthConstraint::loadRelations(QVector<DrawableObject *> list)
{
	QStringList varNames = {
		"originPoint",
		"drivenPoint"
	};

	QVector<DrawableObject*> values = fetchRelations(&list, varNames);

	this->originPoint = dynamic_cast<Point*>(values[0]);
	this->drivenPoint = dynamic_cast<Point*>(values[1]);
	setGeometryUpdates();
}

void LineLengthConstraint::setDistanceFromLine(double distance)
{
	this->distanceFromLine = distance;
}

void LineLengthConstraint::setLength(double length)
{
	lengthToSet = length;
}

QRectF LineLengthConstraint::boundingRect() const
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

QPainterPath LineLengthConstraint::shape() const
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

	polygon << originPoint->getLocation()
			<< aboveStartPoint
			<< aboveEndPoint
			<< drivenPoint->getLocation()
			<< originPoint->getLocation();

	QPainterPath path;
	path.setFillRule(Qt::FillRule::OddEvenFill);
	path.addPolygon(polygon);

	return path;
}

void LineLengthConstraint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void LineLengthConstraint::recieveDouble(double value)
{
	this->lengthToSet = value;
	resolveTies();
}

QVector2D LineLengthConstraint::getLineVector() const
{
	return QVector2D(drivenPoint->getLocation() - originPoint->getLocation());
}

void LineLengthConstraint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->isDraging())
	{
		this->distanceFromLine = -Line::signedDistanceFrom(originPoint->getLocation(), drivenPoint->getLocation(), event->pos());
	}
}

void LineLengthConstraint::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	emit requestDouble(this, "Length");
}

void LineLengthConstraint::setGeometryUpdates()
{
	originPoint->addGeometryUpdate(this);
	drivenPoint->addGeometryUpdate(this);
}

void LineLengthConstraint::unsetGeometryUpdates()
{
	originPoint->removeGeometryUpdate(this);
	drivenPoint->removeGeometryUpdate(this);
}
