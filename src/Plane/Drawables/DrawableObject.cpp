#include "DrawableObject.h"

DrawableObject::DrawableObject(int type)
{
	this->type = type;
	this->propagateGeometry = QVector<DrawableObject*>();
	id = 0;
}

//----------	file handling    ----------

QString DrawableObject::toFileString()
{
    this->file = "";
	this->fileAddVar("id", this->getId());
	this->fileAddVar("constructional", this->constructional);
	return this->file;
}

void DrawableObject::setConstrained(bool value)
{
	constrained = value;
}

bool DrawableObject::isConstrained()
{
	return constrained;
}

//----------	geometry    ----------

void DrawableObject::addGeometryUpdate(DrawableObject *object)
{
	this->propagateGeometry.append(object);
}

void DrawableObject::removeGeometryUpdate(DrawableObject *object)
{
	this->propagateGeometry.removeOne(object);
}

void DrawableObject::removeGeometryUpdates()
{
	unsetGeometryUpdates();
}

//----------	saving    ----------


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

	this->id = values[startIndex].toInt();
	this->constructional = values[startIndex + 1].toBool();

	return values;
}

QVector<DrawableObject*> DrawableObject::fetchRelations(QVector<DrawableObject*>*list, QStringList varNames)
{
	QVector<DrawableObject*> values;
	values.resize(varNames.length());

	QStringList fileVars = this->file.split(',');
	for(int i = 0; i < fileVars.length(); i++)
	{
		QStringList parts = fileVars[i].split(":");
		QString varName = parts[0];
		QString varValue = parts[1];

		int index = varNames.indexOf(varName);
		if(index >= 0)
			values[index] = DrawableObject::getById(list, QVariant(varValue).toInt());
	}

	return values;
}

//----------    array operator    ----------

DrawableObject *DrawableObject::getById(QVector<DrawableObject *> *list, int id)
{
    for(int i = 0; i < list->length(); i++)
    {
        if(list->at(i)->getId() == id)
            return list->at(i);
    }

    return nullptr;
}

//----------	QGraphicsItem overrides    ----------

void DrawableObject::paint(QPainter *painter)
{
	resolveTies();

	QPen currentPen = style->pen;

	if(this->highlight) currentPen.setWidth(currentPen.width() * 2);
	if(this->constructional) currentPen.setStyle(Qt::PenStyle::DashLine);

	painter->setPen(currentPen);

	QBrush currentBrush = style->brush;
	if(type == Global::Types::Point)
		currentBrush.setStyle(Qt::BrushStyle::SolidPattern);

	painter->setBrush(currentBrush);
}

//----------     events     ----------

void DrawableObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	this->draging = true;
}

void DrawableObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	this->draging = false;
}

void DrawableObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	this->highlight = true;
}

void DrawableObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	this->highlight = false;
}

void DrawableObject::updateGeometry()
{
	prepareGeometryChange();
	foreach(DrawableObject *object, this->propagateGeometry)
	{
		object->updateGeometry();
	}
}
