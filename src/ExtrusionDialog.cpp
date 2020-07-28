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

void ExtrusionDialog::show(Item *item)
{
	referencedItem = item;
	for(uint32_t i = 0; i < referencedItem->getPolygons()->size(); i++)
	{
		this->ui->polygonsList->addItem(referencedItem->getPolygons()->at(i));
	}
	QDialog::show();
}

void ExtrusionDialog::selectedFaceChanged()
{
	for(uint32_t i = 0; i < referencedItem->getPolygons()->size(); i++)
	{
		Polygon *polygon = referencedItem->getPolygons()->at(i);
		if(polygon->isSelected()) polygon->setColor(Settings::selectedFaceColor);
	}
}

void ExtrusionDialog::accept()
{
	ExtrusionDirection direction;
	if(ui->frontButton->isChecked()) direction = ExtrusionDirection::Front;
	if(ui->frontAndBackButton->isChecked()) direction = ExtrusionDirection::FrontAndBack;
	if(ui->backButton->isChecked()) direction = ExtrusionDirection::Back;

	std::vector<Polygon*> polygons;
	for(uint32_t i = 0; i < referencedItem->getPolygons()->size(); i++)
	{
		Polygon *polygon = referencedItem->getPolygons()->at(i);
		if(polygon->isSelected())
		{
			polygon->extrude(ui->lengthInput->value(), ui->additiveButton->isChecked(), direction);
		}
	}

	QDialog::accept();
}
