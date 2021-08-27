#ifndef SPACEFACTORY_H
#define SPACEFACTORY_H

//disable CGAL for faster development
#define NO_CGAL

#ifndef NO_CGAL

#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>

#endif

#include "include/delaunator/delaunator.h"

#include "Item.h"
#include "src/Dialogs/MessagesManager.h"

// color normals type
#define FRAG_NORMALS
//#define FACE_NORMALS

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
	std::vector<Vertex> generateBuffer();
	std::vector<Vertex> generateLinesBuffer();

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

	void assignNormals(std::vector<Vertex> *vertexData);
	void orientTriangle(Vertex *v0, Vertex *v1, Vertex *v2, bool up = true);
	std::vector<std::vector<Vertex>> calculateBoolean(const std::vector<std::vector<Vertex>> *triangularizedVertexData) const;

	QByteArray vectorToByteArray(QVector3D vector);

signals:
	void reallocateItems();
	void reallocatePlanes();
	void allocateNewPlane(double planeSize = 20);

	std::vector<QPolygonF> generatePolygons(QString sketch);
	Plane *getBasePlane();
};

#endif // SPACEFACTORY_H
