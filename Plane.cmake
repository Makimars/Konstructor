
set(PLANE
    src/Plane/Plane.h

    src/Plane/PlaneFactory.cpp
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
    src/Plane/PlaneFactory.h
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
