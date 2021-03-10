cmake_minimum_required(VERSION 3.5)

project(KonstructorCmake LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
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



set(PLANE
    src/Plane/Plane.h

    src/Plane/Factory.cpp
    src/Plane/QGraphicsViewUserInput.cpp

    src/Plane/Tools/LockPositionTool.cpp
    src/Plane/Tools/PointPositionTool.cpp
    src/Plane/Tools/CircleTool.cpp
    src/Plane/Tools/RectangleTool.cpp
    src/Plane/Tools/LabelTool.cpp
    src/Plane/Tools/LineTool.cpp
    src/Plane/Tools/ArcTool.cpp
    src/Plane/Tools/LineCenterTool.cpp
    src/Plane/Tools/ExpandPolygonTool.cpp

    src/Plane/Tools/Constraints/CircleRadiusConstraintTool.cpp
    src/Plane/Tools/Constraints/PointDistanceConstrainTool.cpp
    src/Plane/Tools/Constraints/ParaelLinesConstraintTool.cpp
    src/Plane/Tools/Constraints/LinesAngleConstraintTool.cpp

    src/Plane/Drawables/DrawableObject.cpp
    src/Plane/Drawables/Circle.cpp
    src/Plane/Drawables/Line.cpp
    src/Plane/Drawables/Point.cpp
    src/Plane/Drawables/Label.cpp
    src/Plane/Drawables/Arc.cpp

    src/Plane/Drawables/Constraints/ParaelLinesConstraint.cpp
    src/Plane/Drawables/Constraints/PointDistanceConstraint.cpp
    src/Plane/Drawables/Constraints/LineCenterPointConstraint.cpp
    src/Plane/Drawables/Constraints/CircleRadiusConstraint.cpp

    src/Plane/Drawables/Dimensions/CirclesRadiusDifferenceDimension.cpp
    src/Plane/Drawables/Dimensions/LinesAngleDimension.cpp

    src/Plane/Style.h
    src/Plane/Factory.h
    src/Plane/UserInputRequester.h
    src/Plane/QGraphicsViewUserInput.h

    src/Plane/Tools/Tools.h
    src/Plane/Tools/ArcTool.h
    src/Plane/Tools/LockPositionTool.h
    src/Plane/Tools/PointPositionTool.h
    src/Plane/Tools/Tool.h
    src/Plane/Tools/LineTool.h
    src/Plane/Tools/CircleTool.h
    src/Plane/Tools/RectangleTool.h
    src/Plane/Tools/LabelTool.h
    src/Plane/Tools/LineCenterTool.h
    src/Plane/Tools/ExpandPolygonTool.h

    src/Plane/Drawables/Constraints/LineCenterPointConstraint.h
    src/Plane/Tools/Constraints/PointDistanceConstrainTool.h
    src/Plane/Tools/Constraints/CircleRadiusConstraintTool.h
    src/Plane/Tools/Constraints/ParaelLinesConstraintTool.h

    src/Plane/Drawables/Drawables.h
    src/Plane/Drawables/DrawableObject.h
    src/Plane/Drawables/Circle.h
    src/Plane/Drawables/Line.h
    src/Plane/Drawables/Point.h
    src/Plane/Drawables/Label.h
    src/Plane/Drawables/Arc.h

    src/Plane/Drawables/Constraints/Constraints.h

    src/Plane/Drawables/Constraints/ParaelLinesConstraint.h
    src/Plane/Drawables/Constraints/PointDistanceConstraint.h
    src/Plane/Drawables/Constraints/CircleRadiusConstraint.h

    src/Plane/Drawables/Dimensions/CirclesRadiusDifferenceDimension.h
    src/Plane/Drawables/Dimensions/LinesAngleDimension.h
)

set(SPACE
    src/Space/Space.h

    src/Space/SpaceFactory.cpp
    src/Space/Polygon.cpp
    src/Space/Plane.cpp
    src/Space/Item.cpp
    src/Space/Transform3D.cpp
    src/Space/Camera.cpp

    src/Space/SpaceFactory.h
    src/Space/Polygon.h
    src/Space/Plane.h
    src/Space/Item.h
    src/Space/Transform3D.h
    src/Space/Vertex.h
    src/Space/Camera.h
)

set(DIALOGS
    src/Dialogs/AboutDialog.cpp
    src/Dialogs/PointPositionDialog.cpp
    src/Dialogs/ExtrusionDialog.cpp
    src/Dialogs/MessagesManager.cpp
    src/Dialogs/PopUpMessage.cpp
    src/Dialogs/SettingsDialog.cpp
    src/Dialogs/WelcomeDialog.cpp

    src/Dialogs/AboutDialog.h
    src/Dialogs/ExtrusionDialog.h
    src/Dialogs/MessagesManager.h
    src/Dialogs/PopUpMessage.h
    src/Dialogs/SettingsDialog.h
    src/Dialogs/PointPositionDialog.h
    src/Dialogs/WelcomeDialog.h

    src/Dialogs/AboutDialog.ui
    src/Dialogs/PointPositionDialog.ui
    src/Dialogs/ExtrusionDialog.ui
    src/Dialogs/PopUpMessage.ui
    src/Dialogs/SettingsDialog.ui
    src/Dialogs/WelcomeDialog.ui
)

set(PROJECT_SOURCES
    include/delaunator/delaunator.cpp
    include/delaunator/delaunator.h
    include/nlohmann/json.hpp

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
    qt_add_executable(KonstructorCmake
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(KonstructorCmake SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(KonstructorCmake
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(KonstructorCmake PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
