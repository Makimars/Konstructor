#-------------------------------------------------
#
# Project created by QtCreator 2019-09-10T16:42:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Konstructor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    include/delaunator/delaunator.cpp \
\
    src/Plane/Tools/ExpandPolygonTool.cpp \
    src/PlaneWidget.cpp \
    src/SpaceWidget.cpp \
    src/main.cpp \
    src/MainWindow.cpp \
    \
    src/Dialogs/PointPositionDialog.cpp \
    src/Dialogs/ExtrusionDialog.cpp \
    src/Dialogs/MessagesManager.cpp \
    src/Dialogs/PopUpMessage.cpp \
    src/Dialogs/SettingsDialog.cpp \
    src/Dialogs/WelcomeDialog.cpp \
\
    src/Base/Settings.cpp \
\
    src/Plane/Factory.cpp \
    src/Plane/QGraphicsViewUserInput.cpp \
\
    src/Plane/Tools/LockPositionTool.cpp \
    src/Plane/Tools/PointPositionTool.cpp \
    src/Plane/Tools/DimensionTool.cpp \
    src/Plane/Tools/CircleTool.cpp \
    src/Plane/Tools/RectangleTool.cpp \
    src/Plane/Tools/LabelTool.cpp \
    src/Plane/Tools/LineTool.cpp \
    src/Plane/Tools/ArcTool.cpp \
\
    src/Plane/Tools/Constraints/CircleRadiusConstraintTool.cpp \
    src/Plane/Tools/Constraints/PointDistanceConstrainTool.cpp \
\
    src/Plane/Drawables/DrawableObject.cpp \
    src/Plane/Drawables/Circle.cpp \
    src/Plane/Drawables/Line.cpp \
    src/Plane/Drawables/Point.cpp \
    src/Plane/Drawables/Label.cpp \
    src/Plane/Drawables/Arc.cpp \
\
    src/Plane/Drawables/Constraints/CircleRadiusConstraint.cpp \
    src/Plane/Drawables/Constraints/LineLengthConstraint.cpp \
\
    src/Plane/Drawables/Dimensions/CirclesRadiusDifferenceDimension.cpp \
    src/Plane/Drawables/Dimensions/LineLengthDimension.cpp \
    src/Plane/Drawables/Dimensions/LinesAngleDimension.cpp \
\
    src/Adapters/PointAdapter.cpp \
    src/Adapters/Polygonator.cpp \
\
    src/Space/SpaceFactory.cpp \
    src/Space/Polygon.cpp \
    src/Space/Plane.cpp \
    src/Space/Item.cpp \
    src/Space/Transform3D.cpp \
    src/Space/Camera.cpp \


HEADERS += \
    include/delaunator/delaunator.h \
    include/nlohmann/json.hpp \
\
    src/MainWindow.h \
    src/Plane/Tools/ExpandPolygonTool.h \
    src/PlaneWidget.h \
    src/SpaceWidget.h \
\
    src/Dialogs/ExtrusionDialog.h \
    src/Dialogs/MessagesManager.h \
    src/Dialogs/PopUpMessage.h \
    src/Dialogs/SettingsDialog.h \
    src/Dialogs/PointPositionDialog.h \
    src/Dialogs/WelcomeDialog.h \
\
    src/Base/GlobalVariables.h \
    src/Base/Settings.h \
\
    src/Plane/Plane \
    src/Plane/Style.h \
    src/Plane/Factory.h \
    src/Plane/UserInputRequester.h \
    src/Plane/QGraphicsViewUserInput.h \
\
    src/Plane/Tools/Tools.h \
    src/Plane/Tools/ArcTool.h \
    src/Plane/Tools/LockPositionTool.h \
    src/Plane/Tools/PointPositionTool.h \
    src/Plane/Tools/Tool.h \
    src/Plane/Tools/LineTool.h \
    src/Plane/Tools/CircleTool.h \
    src/Plane/Tools/RectangleTool.h \
    src/Plane/Tools/DimensionTool.h \
    src/Plane/Tools/LabelTool.h \
\
    src/Plane/Tools/Constraints/PointDistanceConstrainTool.h \
    src/Plane/Tools/Constraints/CircleRadiusConstraintTool.h \
\
    src/Plane/Drawables/Drawables.h \
    src/Plane/Drawables/DrawableObject.h \
    src/Plane/Drawables/Circle.h \
    src/Plane/Drawables/Line.h \
    src/Plane/Drawables/Point.h \
    src/Plane/Drawables/Label.h \
    src/Plane/Drawables/Arc.h \
\
    src/Plane/Drawables/Constraints/Constraints.h \
\
    src/Plane/Drawables/Constraints/PointDistanceConstraint.h \
    src/Plane/Drawables/Constraints/CircleRadiusConstraint.h \
\
    src/Plane/Drawables/Dimensions/CirclesRadiusDifferenceDimension.h \
    src/Plane/Drawables/Dimensions/LineLengthDimension.h \
    src/Plane/Drawables/Dimensions/LinesAngleDimension.h \
\
    src/Adapters/PointAdapter.h \
    src/Adapters/Polygonator.h \
\
    src/Space/Space \
    src/Space/SpaceFactory.h \
    src/Space/Polygon.h \
    src/Space/Plane.h \
    src/Space/Item.h \
    src/Space/Transform3D.h \
    src/Space/Vertex.h \
    src/Space/Camera.h \


FORMS += \
    src/MainWindow.ui \
    \
    src/Dialogs/PointPositionDialog.ui \
    src/Dialogs/ExtrusionDialog.ui \
    src/Dialogs/PopUpMessage.ui \
    src/Dialogs/SettingsDialog.ui \
    src/Dialogs/WelcomeDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc \
    shaders.qrc
