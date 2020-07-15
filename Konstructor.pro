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
    src/ClassRepository/Space/Plane.cpp \
    src/MainWindow.cpp \
    src/MessagesManager.cpp \
    src/PopUpMessage.cpp \
    src/SettingsDialog.cpp \
    src/View3DWidget.cpp \
    src/main.cpp \
    src/ViewWidget.cpp \
\
    src/ClassRepository/Settings.cpp \
    src/ClassRepository/Exceptions/DrawableAlreadyRestrainedException.cpp \
\
    src/ClassRepository/Plane/Factory.cpp \
    src/ClassRepository/Plane/QGraphicsViewUserInput.cpp \
\
    src/ClassRepository/Plane/Tools/DimensionTool.cpp \
    src/ClassRepository/Plane/Tools/CircleTool.cpp \
    src/ClassRepository/Plane/Tools/RectangleTool.cpp \
    src/ClassRepository/Plane/Tools/LabelTool.cpp \
    src/ClassRepository/Plane/Tools/LineTool.cpp \
\
    src/ClassRepository/Plane/Drawables/DrawableObject.cpp \
    src/ClassRepository/Plane/Drawables/Circle.cpp \
    src/ClassRepository/Plane/Drawables/Line.cpp \
    src/ClassRepository/Plane/Drawables/Point.cpp \
    src/ClassRepository/Plane/Drawables/Label.cpp \
\
    src/ClassRepository/Plane/Drawables/Dimensions/CircleRadiusDimension.cpp \
    src/ClassRepository/Plane/Drawables/Dimensions/CirclesRadiusDifferenceDimension.cpp \
    src/ClassRepository/Plane/Drawables/Dimensions/LineLengthDimension.cpp \
    src/ClassRepository/Plane/Drawables/Dimensions/LinesAngleDimension.cpp \
\
    src/ClassRepository/Space/Item.cpp \
    src/ClassRepository/Space/Transform3D.cpp \
    src/ClassRepository/Space/Camera.cpp \
\
    src/ClassRepository/Space/IntermediateObjects/TransferLine.cpp \
    src/ClassRepository/Space/IntermediateObjects/TransferPoint.cpp \
\
    src/ClassRepository/Space/Tools/DrawTool.cpp

HEADERS += \
    include/delaunator/delaunator.h \
\
    src/ClassRepository/Space/Plane.h \
    src/MainWindow.h \
    src/MessagesManager.h \
    src/PopUpMessage.h \
    src/SettingsDialog.h \
    src/View3DWidget.h \
    src/ViewWidget.h \
\
    src/ClassRepository/GlobalVariables.h \
    src/ClassRepository/Settings.h \
    src/ClassRepository/Exceptions/DrawableAlreadyRestrainedException.h \
\
    src/ClassRepository/Plane/Factory.h \
    src/ClassRepository/Plane/UserInputRequester.h \
    src/ClassRepository/Plane/QGraphicsViewUserInput.h \
\
    src/ClassRepository/Plane/Tools/Tool.h \
    src/ClassRepository/Plane/Tools/LineTool.h \
    src/ClassRepository/Plane/Tools/CircleTool.h \
    src/ClassRepository/Plane/Tools/RectangleTool.h \
    src/ClassRepository/Plane/Tools/DimensionTool.h \
    src/ClassRepository/Plane/Tools/LabelTool.h \
\
    src/ClassRepository/Plane/Drawables/DrawableObject.h \
    src/ClassRepository/Plane/Drawables/Circle.h \
    src/ClassRepository/Plane/Drawables/Line.h \
    src/ClassRepository/Plane/Drawables/Point.h \
    src/ClassRepository/Plane/Drawables/Label.h \
\
    src/ClassRepository/Plane/Drawables/Dimensions/CircleRadiusDimension.h \
    src/ClassRepository/Plane/Drawables/Dimensions/CirclesRadiusDifferenceDimension.h \
    src/ClassRepository/Plane/Drawables/Dimensions/LineLengthDimension.h \
    src/ClassRepository/Plane/Drawables/Dimensions/LinesAngleDimension.h \
\
    src/ClassRepository/Space/Item.h \
    src/ClassRepository/Space/Transform3D.h \
    src/ClassRepository/Space/Vertex.h \
    src/ClassRepository/Space/Camera.h \
\
    src/ClassRepository/Space/IntermediateObjects/TransferLine.h \
    src/ClassRepository/Space/IntermediateObjects/TransferPoint.h \
\
    src/ClassRepository/Space/Tools/DrawTool.h

FORMS += \
    src/MainWindow.ui \
    src/PopUpMessage.ui \
    src/SettingsDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shaders.qrc
