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


set(INCLUDES
    include/delaunator/delaunator.cpp
    include/delaunator/delaunator.h
    include/nlohmann/json.hpp
)

file(GLOB_RECURSE SRC src/*.cpp src/*.h src/*.ui)

set(PROJECT_SOURCES
    ${INCLUDES}
    ${SRC}

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
