#include "ExtrusionDialog.h"
#include "ui_ExtrusionDialog.h"

ExtrusionDialog::ExtrusionDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ExtrusionDialog)
{
	ui->setupUi(this);

	connect(this->ui->polygonsList, &QListWidget::itemSelectionChanged,
			this, &ExtrusionDialog::selectionChanged
			);
	connect(this, &QDialog::finished,
			this, &ExtrusionDialog::finished
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
			Extrusion extrusion;
			extrusion.length = ui->lengthInput->value();
			extrusion.additive = ui->additiveButton->isChecked();
			extrusion.direction = direction;

			referencedItem->setExtrusion(extrusion, polygon);
		}
	}

	QDialog::accept();
}

void ExtrusionDialog::finished(int result)
{
	for(uint32_t i = 0; i < referencedItem->getPolygons()->size(); i++)
	{
		Polygon *polygon = referencedItem->getPolygons()->at(i);
		polygon->setSelected(false);
		this->ui->polygonsList->takeItem(ui->polygonsList->row(polygon));
	}
}
