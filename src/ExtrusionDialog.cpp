#include "ExtrusionDialog.h"
#include "ui_ExtrusionDialog.h"

ExtrusionDialog::ExtrusionDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ExtrusionDialog)
{
	ui->setupUi(this);

	connect(this->ui->polygonsList, &QListWidget::itemSelectionChanged,
			this, &ExtrusionDialog::selectedFaceChanged
			);
}

ExtrusionDialog::~ExtrusionDialog()
{
    delete ui;
}

ExtrusionDialogReturn ExtrusionDialog::exec(Item *item)
{
    referencedItem = item;
	for(uint32_t i = 0; i < referencedItem->getPolygons()->size(); i++)
	{
		this->ui->polygonsList->addItem(referencedItem->getPolygons()->at(i));
	}

    ExtrusionDialogReturn returnData;
    returnData.dialogCode = QDialog::exec();

    ExtrusionDirection direction;
    if(ui->frontButton->isChecked()) direction = ExtrusionDirection::Front;
    if(ui->frontAndBackButton->isChecked()) direction = ExtrusionDirection::FrontAndBack;
    if(ui->backButton->isChecked()) direction = ExtrusionDirection::Back;
    returnData.direction = direction;
    returnData.length = ui->lengthInput->value();
    returnData.extrusion = ui->additiveButton->isChecked();

	return returnData;
}

std::vector<Polygon*> ExtrusionDialog::getSelectedPolygons()
{
	std::vector<Polygon*> polygons;
	for(uint32_t i = 0; i < referencedItem->getPolygons()->size(); i++)
	{
		Polygon *polygon = referencedItem->getPolygons()->at(i);
		if(polygon->isSelected()) polygons.push_back(polygon);
	}

	return polygons;
}

void ExtrusionDialog::selectedFaceChanged()
{
	for(uint32_t i = 0; i < referencedItem->getPolygons()->size(); i++)
	{
		Polygon *polygon = referencedItem->getPolygons()->at(i);
		if(polygon->isSelected()) polygon->setColor(Settings::selectedFaceColor);
	}
}
