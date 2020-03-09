#include "DrawableObject.h"

DrawableObject::DrawableObject(int type)
{
	this->type = type;
}

//----------	file handling    ----------

QString DrawableObject::toFileString()
{
    this->file = "";
    this->fileAddVar("id", this->getId());
    this->fileAddVar("name", this->getName());
	this->fileAddVar("constructional", this->constructional);
    return this->file;
}

//----------	getters and setters    ----------

DrawableObject *DrawableObject::setName(QString name)
{
	this->name = name;

	return this;
}

QString DrawableObject::getName()
{
	return this->name;
}

int DrawableObject::getType()
{
	return this->type;
}

DrawableObject *DrawableObject::setId(unsigned int id)
{
    this->id = id;

	return this;
}

unsigned int DrawableObject::getId()
{
	return this->id;
}

DrawableObject *DrawableObject::setBrush(QBrush * value)
{
	brush = value;

	return this;
}

QBrush * DrawableObject::getBrush()
{
	return brush;
}

DrawableObject *DrawableObject::setPen(QPen * value)
{
	pen = value;

	return this;
}

QPen * DrawableObject::getPen()
{
	return pen;
}

DrawableObject *DrawableObject::setIsConstructional(bool value)
{
	this->constructional = value;
}

bool DrawableObject::isConstructional()
{
	return this->constructional;
}

DrawableObject *DrawableObject::setHighlight(bool value)
{
	this->highlight = value;
}

bool DrawableObject::isHighlighted()
{
	return this->highlight;
}

DrawableObject *DrawableObject::setHidden(bool value)
{
	this->hidden = value;
}

bool DrawableObject::isHidden()
{
	return this->hidden;
}

//----------	saving    ----------

void DrawableObject::fileAddVar(QString variable, QString value)
{
	this->file += variable + ":" + value + ",";
}

void DrawableObject::fileAddVar(QString variable, double value)
{
    this->fileAddVar(variable, QString::number(value));
}

void DrawableObject::fileAddVar(QString variable, int value)
{
    this->fileAddVar(variable, QString::number(value));
}

void DrawableObject::fileAddVar(QString variable, long value)
{
    this->fileAddVar(variable, QString::number(value));
}

void DrawableObject::fileAddVar(QString variable, unsigned int value)
{
    this->fileAddVar(variable, QString::number(value));
}

void DrawableObject::fileAddVar(QString variable, bool value)
{
    this->fileAddVar(variable, QString::number(value));
}

QString DrawableObject::fileFinish()
{
	this->file = QString::number(this->type) + "{" + this->file.remove(this->file.length()-1, 1) + "};";
	return this->file;
}

//----------     loading objects     ----------

QVector<QVariant> DrawableObject::fetchVariables(QString input, QStringList varNames)
{
	this->file = input;

	int startIndex = varNames.length();
	varNames.append("id");
	varNames.append("name");
	varNames.append("constructional");

	QVector<QVariant> values;
	values.resize(varNames.length());

	QStringList variables = input.split(',');
	for(int i = 0; i < variables.length(); i++)
	{
		QStringList parts = variables[i].split(":");
		QString varName = parts[0];
		QString varValue = parts[1];

		int index = varNames.indexOf(varName);
		if(index >= 0)
			values[index] = QVariant(varValue);
	}

	this->id = values[startIndex].toUInt();
	this->name = values[startIndex + 1].toString();
	this->constructional = values[startIndex + 2].toBool();

	return values;
}

QVector<DrawableObject*> DrawableObject::fetchRelations(QVector<DrawableObject*>*list, QStringList varNames)
{
	QVector<DrawableObject*> values;
	values.resize(varNames.length());

	QStringList fileVars = this->getFile().split(',');
	for(int i = 0; i < fileVars.length(); i++)
	{
		QStringList parts = fileVars[i].split(":");
		QString varName = parts[0];
		QString varValue = parts[1];

		int index = varNames.indexOf(varName);
		if(index >= 0)
			values[index] = DrawableObject::getById(list, QVariant(varValue).toUInt());
	}

	return values;
}

//----------     getters and setters      ----------

QString DrawableObject::getFile()
{
	return this->file;
}

bool DrawableObject::isDraging()
{
	return  this->draging;
}

//----------    array operator    ----------

DrawableObject *DrawableObject::getById(QVector<DrawableObject *> *list, unsigned int id)
{
    for(int i = 0; i < list->length(); i++)
    {
        if(list->at(i)->getId() == id)
            return list->at(i);
    }

    return nullptr;
}

//----------	QGraphicsItem overrides    ----------

QRectF DrawableObject::boundingRect() const
{
	return QRectF();
}

void DrawableObject::paint(QPainter *painter,
						   const QStyleOptionGraphicsItem *option,
						   QWidget *widget)
{
	resolveTies();

	if(this->highlight)
		this->pen->setWidth(2);
	else
		this->pen->setWidth(1);

	painter->setPen(*this->pen);
}

//----------     events     ----------

void DrawableObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << "Mouse press on " + QString::number(this->getType());
	this->draging = true;
}

void DrawableObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	this->draging = false;
}

void DrawableObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	qDebug() << "hover enter" + QString::number(this->getType());
	this->highlight = true;
}

void DrawableObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	this->highlight = false;
}
