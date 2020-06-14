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
    src/MainWindow.cpp \
    src/MessagesManager.cpp \
    src/SettingsDialog.cpp \
    src/View3DWidget.cpp \
    src/main.cpp \
    src/ViewWidget.cpp \
\
    src/ClassRepository/Settings.cpp \
    src/ClassRepository/Exceptions/DrawableAlreadyRestrainedException.cpp \
\
    src/ClassRepository/2D/Factory.cpp \
    src/ClassRepository/2D/QGraphicsViewUserInput.cpp \
\
    src/ClassRepository/2D/Tools/DimensionTool.cpp \
    src/ClassRepository/2D/Tools/CircleTool.cpp \
    src/ClassRepository/2D/Tools/RectangleTool.cpp \
    src/ClassRepository/2D/Tools/LabelTool.cpp \
    src/ClassRepository/2D/Tools/LineTool.cpp \
\
    src/ClassRepository/2D/Drawables/DrawableObject.cpp \
    src/ClassRepository/2D/Drawables/Circle.cpp \
    src/ClassRepository/2D/Drawables/Line.cpp \
    src/ClassRepository/2D/Drawables/Point.cpp \
    src/ClassRepository/2D/Drawables/Label.cpp \
\
    src/ClassRepository/2D/Drawables/Dimensions/CircleRadiusDimension.cpp \
    src/ClassRepository/2D/Drawables/Dimensions/CirclesRadiusDifferenceDimension.cpp \
    src/ClassRepository/2D/Drawables/Dimensions/LineLengthDimension.cpp \
    src/ClassRepository/2D/Drawables/Dimensions/LinesAngleDimension.cpp \
\
    src/ClassRepository/3D/Item.cpp \
    src/ClassRepository/3D/Mesh.cpp \
    src/ClassRepository/3D/Transform3D.cpp \
\
    src/ClassRepository/3D/Tools/DrawTool.cpp

HEADERS += \
    src/MainWindow.h \
    src/MessagesManager.h \
    src/SettingsDialog.h \
    src/View3DWidget.h \
    src/ViewWidget.h \
\
    src/ClassRepository/GlobalVariables.h \
    src/ClassRepository/Settings.h \
    src/ClassRepository/Exceptions/DrawableAlreadyRestrainedException.h \
\
    src/ClassRepository/2D/Factory.h \
    src/ClassRepository/2D/UserInputRequester.h \
    src/ClassRepository/2D/QGraphicsViewUserInput.h \
\
    src/ClassRepository/2D/Tools/Tool.h \
    src/ClassRepository/2D/Tools/LineTool.h \
    src/ClassRepository/2D/Tools/CircleTool.h \
    src/ClassRepository/2D/Tools/RectangleTool.h \
    src/ClassRepository/2D/Tools/DimensionTool.h \
    src/ClassRepository/2D/Tools/LabelTool.h \
\
    src/ClassRepository/2D/Drawables/DrawableObject.h \
    src/ClassRepository/2D/Drawables/Circle.h \
    src/ClassRepository/2D/Drawables/Line.h \
    src/ClassRepository/2D/Drawables/Point.h \
    src/ClassRepository/2D/Drawables/Label.h \
\
    src/ClassRepository/2D/Drawables/Dimensions/CircleRadiusDimension.h \
    src/ClassRepository/2D/Drawables/Dimensions/CirclesRadiusDifferenceDimension.h \
    src/ClassRepository/2D/Drawables/Dimensions/LineLengthDimension.h \
    src/ClassRepository/2D/Drawables/Dimensions/LinesAngleDimension.h \
\
    src/ClassRepository/3D/Item.h \
    src/ClassRepository/3D/Mesh.h \
    src/ClassRepository/3D/Transform3D.h \
    src/ClassRepository/3D/Vertex.h \
\
    src/ClassRepository/3D/Tools/Tool.h \
    src/ClassRepository/3D/Tools/DrawTool.h

FORMS += \
    src/MainWindow.ui \
    src/SettingsDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CodeStyle.md

RESOURCES += \
    shaders.qrc
