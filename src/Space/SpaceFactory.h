#ifndef SPACEFACTORY_H
#define SPACEFACTORY_H

#include "Item.h"

#include "include/delaunator/delaunator.h"
#include "include/libigl/include/igl/cotmatrix.h"

class SpaceFactory : public QObject
{
	Q_OBJECT
public:
	static void init(QVector<Item*> *objectsInSpace,
						QVector<Plane*> *planes,
						QObject *glWidget
						);
	static SpaceFactory *getInstance();

public:
	void generateItemVertexDataToBuffer(Item *item, std::vector<Vertex> *vertexData);
	QByteArray generateStlFile(std::vector<Vertex> *vertexData);

public slots:
	void recieveTargetItem(QTreeWidgetItem *item);

	Item *loadItem(std::string file);

	void addNewItem(std::vector<QPolygonF> polygons, QString sketch);
	void addItem(Item *item);
	void deleteItem(Item *item);
	void deleteAllItems();

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

	std::vector<QPolygonF> generatePolygons(QString sketch);
	Plane *getBasePlane();
};

#endif // SPACEFACTORY_H
