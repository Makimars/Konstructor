cmake_minimum_required(VERSION 3.5)

project(Konstructor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

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

include(Plane.cmake)
include(Space.cmake)
include(Dialogs.cmake)

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
    if(ANDROID)
	add_library(Konstructor SHARED
            ${PROJECT_SOURCES}
        )
    else()
	add_executable(Konstructor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Konstructor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Konstructor PRIVATE CGAL)
target_link_libraries(Konstructor PRIVATE pthread)
#target_link_libraries(Konstructor PRIVATE eigen3/Eigen)
