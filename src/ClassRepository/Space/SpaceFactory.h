#ifndef SPACEFACTORY_H
#define SPACEFACTORY_H

#include "Item.h"

class SpaceFactory : public QObject
{
	Q_OBJECT
public:
	static void init(QVector<Item*> *objectsInSpace,
						QVector<Plane*> *planes,
						QObject *glWidget
						);
	static SpaceFactory *getInstance();

	void loadFromFile(QString file);

public slots:
	void recieveTargetItem(QTreeWidgetItem *item);

	void addItem(std::vector<QPolygonF> polygons, QString sketch);
	void deleteItem(Item *item);

	void addPlane(Plane *plane);
	void deletePlane(Plane *plane);

private:
	SpaceFactory(QVector<Item*> *objectsInSpace,
				 QVector<Plane*> *planes,
				 QObject *glWidget
				 );
	static SpaceFactory *instance;

	QTreeWidgetItem *targetItem;

	QVector<Item*> *objectsInSpace;
	QVector<Plane*> *planes;
	QObject *glWidget;

signals:
	void reallocateItems();
	void allocateNewItem(Item *item);
	void reallocatePlanes();
	void allocateNewPlane();
};

#endif // SPACEFACTORY_H
