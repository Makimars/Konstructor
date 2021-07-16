#include "DrawableObject.h"

DrawableObject::DrawableObject(int type)
{
	this->type = type;
	this->propagateGeometry = QVector<DrawableObject*>();
	id = 0;
}

//----------	file handling    ----------

void DrawableObject::loadData(nlohmann::json jsonInput)
{
	this->id = jsonInput["id"];
	this->constructional = jsonInput["constructional"];
	this->locked = jsonInput["locked"];
	this->constrains = jsonInput["constraints"];

	this->json = jsonInput;
}

nlohmann::json DrawableObject::toJson()
{
	json.clear();
	json["id"] = this->getId();
	json["constructional"] = this->constructional;
	json["locked"] = this->locked;
	json["constraints"] = this->constrains;

	json["type"] = this->type;

	return json;
}

void DrawableObject::addConstraint()
{
	constrains++;
}

void DrawableObject::removeConstraint()
{
	constrains--;
}

bool DrawableObject::isConstrained()
{
	return constrains > 0;
}

bool DrawableObject::hasGeometryUpdates()
{
	return propagateGeometry.size() > 0;
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

//----------     loading objects     ----------

QVector<DrawableObject*> DrawableObject::fetchRelations(QVector<DrawableObject*>*list, QStringList varNames)
{
	QVector<DrawableObject*> values;
	values.resize(varNames.length());

	for(int i = 0; i < varNames.length(); i++)
	{
		values[i] = DrawableObject::getById(list, this->json[varNames.at(i).toStdString()]);
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
	if((event->button() == Qt::MouseButton::LeftButton) & !this->isLocked() & !this->isConstrained())
		this->draging = true;
}

void DrawableObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button() == Qt::MouseButton::LeftButton)
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
