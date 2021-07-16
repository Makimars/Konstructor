#include "Label.h"

Label::Label() : DrawableObject (Global::Label){}

Label::Label(QPointF location, QString text) : Label()
{
	this->location = location;
	this->text = text;
}

//---------    file handeling     ---------

void Label::loadData(nlohmann::json jsonInput)
{
	DrawableObject::loadData(jsonInput);
	this->text.fromStdString(jsonInput["text"]);
	this->location = QPointF(jsonInput["x"], jsonInput["y"]);
}

nlohmann::json Label::toJson()
{
	DrawableObject::toJson();
	json["x"] = location.x();
	json["y"] = location.y();
	json["text"] = text.toStdString();

	return json;
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
	QRectF rect(
				this->location - QPointF(this->textWidth * text.size() / 2, this->textHeight / 2),
				QSizeF(this->textWidth * text.size(), this->textHeight)
			);

	return rect;
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
