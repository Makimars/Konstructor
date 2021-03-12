#ifndef SPACEFACTORY_H
#define SPACEFACTORY_H

#define NO_IGL_BOOLEAN

#include "Item.h"

#include "include/delaunator/delaunator.h"

#ifndef NO_IGL_BOOLEAN
// Igl
#include "include/libigl/include/igl/cotmatrix.h"
#include "include/libigl/include/igl/copyleft/cgal/mesh_boolean.h"
#endif

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
	std::vector<Vertex> generateBuffer();

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

	std::vector<Vertex> triangularizePolygon(QPolygonF polygon);
	std::vector<Vertex> triangularizeItem(Item *item);

#ifndef NO_IGL_BOOLEAN
	std::vector<std::vector<Vertex>> calculateBoolean(const std::vector<std::vector<Vertex>> *triangularizedVertexData) const;
#endif

	QByteArray vectorToByteArray(QVector3D vector);

signals:
	void reallocateItems();
	void reallocatePlanes();
	void allocateNewPlane();

	std::vector<QPolygonF> generatePolygons(QString sketch);
	Plane *getBasePlane();
};

#endif // SPACEFACTORY_H
