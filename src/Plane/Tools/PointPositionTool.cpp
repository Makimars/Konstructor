#include "PointPositionTool.h"

PointPositionTool *PointPositionTool::instance = nullptr;

PointPositionTool::PointPositionTool() : Tool()
{
	dialog = new PointPositionDialog();

	toolTips.append(tr("Select point to set its coordinates."));
}

PointPositionTool::~PointPositionTool()
{
	delete dialog;
}

PointPositionTool *PointPositionTool::getInstance()
{
	if(PointPositionTool::instance == nullptr)
		PointPositionTool::instance = new PointPositionTool();
	return PointPositionTool::instance;
}

void PointPositionTool::click(DrawableObject *clickedObject, QPointF pos)
{
	if(Point *point = dynamic_cast<Point*>(clickedObject))
	{
		dialog->position = point->getLocation();
		int res = dialog->exec();

		switch(res)
		{
			case QDialog::DialogCode::Accepted:
				point->setLocation(dialog->position);
				break;
			case QDialog::DialogCode::Rejected:
			default:
				//nothing happens
				break;
		}
	}
}
