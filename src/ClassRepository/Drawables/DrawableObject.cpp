#include "DrawableObject.h"

DrawableObject::DrawableObject()
{
	this->type = "DrawableObject";
}

DrawableObject::~DrawableObject()
{

}

void DrawableObject::resolveTies()
{

}

//----------	file handling    ----------

//input format: name:value,type:value,
void DrawableObject::fromFileString(QString input)
{
    this->file = input;

    QStringList var_names = {
        "id",
        "name",
		"constructional"
    };

	QStringList variables = input.split(',');
    for(int i = 0; i < variables.length() - 1; i++)
	{
		QStringList parts = variables[i].split(":");
		QString var_name = parts[0];
		QString var_value = parts[1];

		switch (var_names.indexOf(var_name)) {
			case 0:
                this->id = QVariant(var_value).toUInt();
				break;
			case 1:
				this->name = var_value;
				break;
			case 2:
				this->constructional = QVariant(var_value).toBool();
				break;
			default:
				break;
		}

    }
}

QString DrawableObject::toFileString()
{
    this->file = "";
    this->fileAddVar("id", this->getId());
    this->fileAddVar("name", this->getName());
	this->fileAddVar("constructional", this->constructional);
    return this->file;
}

void DrawableObject::loadRelations(QVector<DrawableObject *> *list)
{

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

QString DrawableObject::getType()
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

void DrawableObject::setIsConstructional(bool value)
{
	this->constructional = value;
}

bool DrawableObject::isConstructional()
{
	return this->constructional;
}

void DrawableObject::setHighlight(bool value)
{
	this->highlight = value;
}

bool DrawableObject::isHighlighted()
{
	return this->highlight;
}

void DrawableObject::setHidden(bool value)
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
	this->file = this->type + "{" + this->file + "};";
    return this->file;
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

	if(this->highlight){
		this->pen->setWidth(2);
		painter->setPen(*this->pen);
	}
}

void DrawableObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << "mousepress on drawable" + type;
}

