#include "SpaceFactory.h"

SpaceFactory *SpaceFactory::instance = nullptr;

void SpaceFactory::init(QVector<Item*> *objectsInSpace, QVector<Plane*> *planes, QObject *glWidget)
{
	SpaceFactory::instance = new SpaceFactory(objectsInSpace, planes, glWidget);
}

SpaceFactory *SpaceFactory::getInstance()
{
	return SpaceFactory::instance;
}

void SpaceFactory::recieveTargetItem(QTreeWidgetItem *item)
{
	targetItem = item;
}

void SpaceFactory::addItem(std::vector<QPolygonF> polygons, QString sketch)
{
	if(Plane *plane = dynamic_cast<Plane*>(targetItem))
	{
		Item *item = new Item(plane, polygons, sketch);

		item->setText(0, "object " + QString::number(objectsInSpace->size()));

		objectsInSpace->append(item);

		connect(item, &Item::updateData,
				this, &SpaceFactory::reallocateItems
				);
		connect(item, &Item::planeAdded,
				this, &SpaceFactory::addPlane
				);
		connect(item, &Item::deletePlane,
				this, &SpaceFactory::deletePlane
				);

		emit allocateNewItem(item);
	}
	else if(Item *item = dynamic_cast<Item*>(targetItem))
	{
		item->setPolygons(polygons);
		item->setSketch(sketch);

		emit reallocateItems();
	}
}

void SpaceFactory::deleteItem(Item *item)
{
	objectsInSpace->remove(objectsInSpace->indexOf(item));
	delete item;
	emit reallocateItems();
}

void SpaceFactory::addPlane(Plane *plane)
{
	planes->append(plane);
	plane->setItemIndex(objectsInSpace->indexOf(dynamic_cast<Item*>(plane->parent())));

	emit allocateNewPlane();
}

void SpaceFactory::deletePlane(Plane *plane)
{
	if(plane == nullptr) return;

	for (int i = 0; i < plane->childCount(); i++)
	{
		if(Item *item = dynamic_cast<Item*>(plane->child(i)))
			deleteItem(item);
	}

	planes->removeAll(plane);
	delete plane;
}

SpaceFactory::SpaceFactory(QVector<Item*> *objectsInSpace, QVector<Plane*> *planes, QObject *glWidget)
{
	this->objectsInSpace = objectsInSpace;
	this->planes = planes;
	this->glWidget = glWidget;
}
