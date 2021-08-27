#ifndef SPACEFACTORY_H
#define SPACEFACTORY_H

#include "SpaceProcessor.h"

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
	QByteArray generateStlFile(std::vector<Vertex> *vertexData);
	void generateOffFile(std::vector<Vertex> *vertexData, QString filePath);

	std::vector<Vertex> generateItemBuffer();
	std::vector<Vertex> generateLinesBuffer();
	std::vector<Vertex> generatePlanesBuffer();

public slots:
	void recieveTargetItem(QTreeWidgetItem *item);

	Item *loadItem(std::string file);

	void addNewItem(std::vector<QPolygonF> polygons, QString sketch);

	/** @brief adds item to list and connects, reallocates if selected */
	void addItem(Item *item, bool reallocate = true);

	/** @brief removes the itme from list, deletes and reallocates if selected*/
	void deleteItem(Item *item, bool reallocate = true);

	/** @brief delet all items and then reallocate */
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

	QVector<Item*> *itemsInSpace;
	QVector<Plane*> *planes;
	QObject *glWidget;

	QByteArray vectorToByteArray(QVector3D vector);

signals:
	void reallocateItems();
	void reallocatePlanes();
	void allocateNewPlane(double planeSize = 20);

	std::vector<QPolygonF> generatePolygons(QString sketch);
	Plane *getBasePlane();
};

#endif // SPACEFACTORY_H
