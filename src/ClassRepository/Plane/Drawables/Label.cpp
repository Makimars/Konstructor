#include "Label.h"

Label::Label() : DrawableObject (Global::Label){}

Label::Label(QPointF location) : DrawableObject (Global::Label)
{
	this->location = location;
}

Label::Label(QPointF location, QString text) : DrawableObject (Global::Label)
{
	this->location = location;
	this->text = text;
}

//---------    file handeling     ---------

void Label::loadVariables(QString input)
{
	QStringList varNames = {
		"text",
		"x",
		"y"
	};

	QVector<QVariant> variables = fetchVariables(input, varNames);

	this->text = variables[0].toString();
	this->location = QPointF(variables[1].toDouble(), variables[2].toDouble());
}

QString Label::toFileString()
{
	DrawableObject::toFileString();
	this->fileAddVar("text", this->text);
	this->fileAddVar("x", this->location.x());
	this->fileAddVar("y", this->location.y());
	return this->fileFinish();
}

//----------	getters and setters    ----------

QString Label::getText()
{
	return this->text;
}

void Label::setText(QString text)
{
	this->text = text;
}

QPointF Label::getLocation()
{
	return this->location;
}

void Label::setLocation(QPointF location)
{
	this->location = location;
}

void Label::requestText()
{
	emit requestString(this, "Text");
}

//----------	QGraphicsItem overrides    ----------

QRectF Label::boundingRect() const
{
	return QRectF(this->location-QPointF(this->textWidth/2,this->textHeight/2),QSizeF(this->textWidth, this->textHeight));
}

void Label::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(this->isHidden())return;

	DrawableObject::paint(painter);

	painter->drawRect(boundingRect());
	painter->drawText(boundingRect(), this->text);
}

//---------     events     ----------

void Label::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->isDraging())
	{
		prepareGeometryChange();
		this->location = event->pos();
	}
}

void Label::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	emit requestString(this, "Text");
}

//--------     user input requests     ---------

void Label::recieveString(QString value)
{
	this->text = value;
}
