#ifndef SPACEFACTORY_H
#define SPACEFACTORY_H

#include "Item.h"

#include "include/delaunator/delaunator.h"

//#define IGL_BOOLEAN
#ifdef IGL_BOOLEAN
// Igl
#include "include/libigl/include/igl/cotmatrix.h"
#include "include/libigl/include/igl/copyleft/cgal/mesh_boolean.h"
#endif

//#define CGAL_BOOLEAN
#ifdef CGAL_BOOLEAN
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_items_with_id_3.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>

#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/boost/graph/selection.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
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


#if defined(CGAL_BOOLEAN) || defined(CGAL_BOOLEAN)
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
