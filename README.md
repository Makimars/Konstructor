# Konstructor

CAD written in C++ and Qt

The purpose of this project is to create simple free and open source CAD with support for drawing part planes and extruding them to a 3D shape.

This software is linced under EUPL v1.2, other language versions are located in folder LICENCE, and are equally valid.

## Dependencies

- JSON for modern C++ (included)
- delaunator-cpp (included)
- CGAL
- QT 5

## current TODO list

### Plane

- fix side projection
- rewok constraints to use equasions (low priority)

### Polygonator

- better polygon finder

### Space

- cube in the right top corner to select view
- fix side planes
- better polygon viewer
- unified internal data format
- separate data managment to SpaceFactory
