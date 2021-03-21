cmake_minimum_required(VERSION 3.5)

project(Konstructor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(CGAL)
find_package(pthread)
#find_package(eigen3/Eigen)

file(GLOB_RECURSE IGL_SRC include/igl/include/*.cpp include/igl/include/*.h)

set(INCLUDES
    ${IGL_SRC}

    include/delaunator/delaunator.cpp
    include/delaunator/delaunator.h
    include/nlohmann/json.hpp
)

file(GLOB_RECURSE PLANE src/Plane/*.cpp src/Plane/*.h)
file(GLOB_RECURSE SPACE src/Space/*.cpp src/Space/*.h)
file(GLOB_RECURSE DIALOGS src/Dialogs/*.cpp src/Dialogs/*.h src/Dialogs/*.ui)

set(PROJECT_SOURCES
    ${INCLUDES}
    ${PLANE}
    ${SPACE}
    ${DIALOGS}

    src/main.cpp

    src/MainWindow.cpp
    src/MainWindow.h
    src/MainWindow.ui

    src/PlaneWidget.cpp
    src/PlaneWidget.h

    src/SpaceWidget.cpp
    src/SpaceWidget.h

    src/Base/Settings.cpp
    src/Base/GlobalVariables.h
    src/Base/Settings.h

    src/Adapters/PointAdapter.cpp
    src/Adapters/PointAdapter.h
    src/Adapters/Polygonator.cpp
    src/Adapters/Polygonator.h

    icons.qrc
    shaders.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Konstructor
        ${PROJECT_SOURCES}
    )
else()
    add_executable(Konstructor
	${PROJECT_SOURCES}
    )
endif()

target_link_libraries(Konstructor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Konstructor PRIVATE CGAL)
target_link_libraries(Konstructor PRIVATE pthread)
#target_link_libraries(Konstructor PRIVATE eigen3/Eigen)
