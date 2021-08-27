#ifndef SPACEPROCESSOR_H
#define SPACEPROCESSOR_H

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

// color normals type
#define FRAG_NORMALS
//#define FACE_NORMALS


#include "include/delaunator/delaunator.h"

#include "Item.h"
#include "src/Dialogs/MessagesManager.h"

//transforms raw Items and Planes to triangularized data ready to be rendered
class SpaceProcessor
{
public:
	static SpaceProcessor *getInstance();

	std::vector<Vertex> generateItemBuffer(const QVector<Item*> *items);
	std::vector<Vertex> generateLinesBuffer(const QVector<Item*> *items);
	std::vector<Vertex> generatePlanesBuffer(const QVector<Plane*> *planes);

private:
	SpaceProcessor();
	static SpaceProcessor *instance;

	std::vector<Vertex> triangularizePolygon(QPolygonF polygon);
	std::vector<Vertex> triangularizeItem(Item *item);

	void assignNormals(std::vector<Vertex> *vertexData);
	void orientTriangle(Vertex *v0, Vertex *v1, Vertex *v2, bool up = true);
	std::vector<std::vector<Vertex>> calculateBoolean(const std::vector<std::vector<Vertex>> *triangularizedVertexData) const;

};

#endif // SPACEPROCESSOR_H
