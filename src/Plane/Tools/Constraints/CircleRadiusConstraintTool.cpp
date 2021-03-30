#include "CircleRadiusConstraintTool.h"

CircleRadiusConstraintTool *CircleRadiusConstraintTool::instance = nullptr;

CircleRadiusConstraintTool::CircleRadiusConstraintTool() : Tool()
{
	toolTips.append(tr("Select a circle"));
}

CircleRadiusConstraintTool *CircleRadiusConstraintTool::getInstance()
{
	if(CircleRadiusConstraintTool::instance == nullptr)
		CircleRadiusConstraintTool::instance = new CircleRadiusConstraintTool();
	return CircleRadiusConstraintTool::instance;
}

void CircleRadiusConstraintTool::click(DrawableObject *clickedObject, QPointF pos)
{
	if(clickedObject != nullptr)
	{
		if(clickedObject->getType() == Global::Types::Circle)
		{
			Circle *circle = dynamic_cast<Circle*>(clickedObject);

			planeFactory->addDrawable(
				planeFactory->makeCircleRadiusConstraint(circle)
			);
		}
	}
}

void CircleRadiusConstraintTool::resetTool()
{

}
