#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	loadSettings();

	this->ui->setupUi(this);
	this->setupUi();
	this->setupConnections();

	this->settingsDialog = new SettingsDialog();
	this->extrusionDialog = new ExtrusionDialog();
	MessagesManager::init();

	this->swapMode(Global::Mode::Object);
}

MainWindow::~MainWindow()
{
	delete this->ui;
}

void MainWindow::setupUi()
{
    //object menu
	this->ui->objectsTree->setHeaderHidden(true);
    ui->objectsTree->setContextMenuPolicy(Qt::CustomContextMenu);

	Space::Plane *basePlane = new Space::Plane();
	basePlane->setPosition(QVector3D(0,0,0));
	basePlane->setRotation(QQuaternion(0,0,0,0));
	basePlane->setText(0, "origin plane");
    this->ui->objectsTree->addTopLevelItem(basePlane);

    //context menus
	drawAction.setText(tr("Draw"));
    planeContextMenu.addAction(&drawAction);

	extrusionAction.setText(tr("Extrude"));
	objectContextMenu.addAction(&extrusionAction);
	redrawAction.setText(tr("Redraw"));
	objectContextMenu.addAction(&redrawAction);
}

void MainWindow::setupConnections()
{
	//forward keypresses to ViewWidget
	connect(this->ui->view2D, &ViewWidget::keyPressed,
			this, &MainWindow::viewKeyPress
			);
	//forwards statusBar messages from VieewWidget
	connect(this->ui->view2D, &ViewWidget::showStatusBarMessage,
			this->ui->statusBar, &QStatusBar::showMessage
			);

	//tools
	connect(this, &MainWindow::setTool,
			this->ui->view2D, &ViewWidget::setTool
			);
	connect(this, &MainWindow::resetTool,
			this->ui->view2D, &ViewWidget::resetTool
			);

	//sketch file
	connect(this->ui->newSketchButton, &QToolButton::clicked,
			this->ui->view2D, &ViewWidget::newSketchButtonClicked
			);
	connect(this->ui->closeSketchButton, &QToolButton::clicked,
			this->ui->view2D, &ViewWidget::closeSketchButtonClicked
			);

	//Plane to Space connection
	//set item on which to draw
	connect(this, &MainWindow::setTargetItem,
			this->ui->view3D, &View3DWidget::recieveTargetItem
			);
	//send finishWarning signal to ViewWidget
	connect(this, &MainWindow::finishDrawing,
			this->ui->view2D, &ViewWidget::finishDrawing
			);
	//sends drawing to polygonator
	connect(this->ui->view2D, &ViewWidget::returnDrawing,
			Polygonator::getInstance(), &Polygonator::recieveDrawing
			);
	//sends polygons from polygonator to View3DWidget
	connect(Polygonator::getInstance(), &Polygonator::sendPolygons,
			this->ui->view3D, &View3DWidget::addItem
			);
}

//----------    Ui handeling    ---------

void MainWindow::refreshTools(int tool)
{
	if(tool != Global::Tools::LineTool)
		this->ui->lineButton->setChecked(false);
	if(tool != Global::Tools::CircleTool)
		this->ui->circleButton->setChecked(false);
	if(tool != Global::Tools::RectangleTool)
		this->ui->rectangleButton->setChecked(false);
	if(tool != Global::Tools::LabelTool)
		this->ui->labelButton->setChecked(false);
	if(tool != Global::Tools::DimensionTool)
		this->ui->dimensionButton->setChecked(false);

	emit setTool(tool);
}

//----------	settings    ----------

void MainWindow::loadSettings()
{

}

void MainWindow::saveSettings()
{

}


//----------	ui signals reciever    ----------
//-----    file tab    -----


void MainWindow::on_newObjectFile_clicked()
{

}

void MainWindow::on_openObjectFile_clicked()
{

}

void MainWindow::on_saveObjectButton_clicked()
{

}

void MainWindow::on_exportObjectButton_clicked()
{

}

void MainWindow::on_settingsButton_clicked()
{
	this->settingsDialog->exec();
}

void MainWindow::on_quitButton_clicked()
{
	this->close();
}

//-----    sketch files    -----

void MainWindow::on_closeSketchButton_clicked()
{
	swapMode(Global::Mode::Object);
}

void MainWindow::on_saveSketchButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(
			this,
			Global::saveFile,
			Settings::userProjectRoot,
			Global::konstructorSketch + ";;" + Global::allFiles
			);
	this->ui->view2D->saveToFile(fileName);
}

void MainWindow::on_openSketchButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(
			this,
					Global::openFile,
			Settings::userProjectRoot,
			Global::konstructorSketch + ";;" + Global::allFiles
			);

	QFile file(fileName);
	if(file.exists())
	{
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
			this->ui->view2D->loadFromFile(file.readAll());
	}
}

//-----    sketch tools    -----

void MainWindow::on_lineButton_clicked()
{
	if(this->ui->lineButton->isChecked())
		refreshTools(Global::Tools::LineTool);
	else
		refreshTools(Global::Tools::NoTool);
}

void MainWindow::on_circleButton_clicked()
{

	if(this->ui->circleButton->isChecked())
		refreshTools(Global::Tools::CircleTool);
	else
		refreshTools(Global::Tools::NoTool);
}

void MainWindow::on_rectangleButton_clicked()
{
	if(this->ui->rectangleButton->isChecked())
		refreshTools(Global::Tools::RectangleTool);
	else
		refreshTools(Global::Tools::NoTool);
}

void MainWindow::on_labelButton_clicked()
{
	if(this->ui->labelButton->isChecked())
		refreshTools(Global::Tools::LabelTool);
	else
		refreshTools(Global::Tools::NoTool);
}

//-----    constrains    -----

void MainWindow::on_dimensionButton_clicked()
{
	if(this->ui->dimensionButton->isChecked())
		refreshTools(Global::Tools::DimensionTool);
	else
		refreshTools(Global::Tools::NoTool);
}

//-----    finish drawing    -----

void MainWindow::on_finishDrawingButton_clicked()
{
	if(this->ui->objectsTree->currentItem() == nullptr)
	{
		if(MessagesManager::showOkCancelForm("No plane selected") == QDialog::Rejected)
			return;
	}

	swapMode(Global::Mode::Object);
	emit finishDrawing();
}

//-----    misc slots    -----

void MainWindow::on_objectsTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    swapMode(Global::Mode::Draw);
    emit setTargetItem(item);

    if(Item *existingItem = dynamic_cast<Item*>(item))
    {
		this->ui->view2D->loadFromFile(existingItem->getSketch());
	}
}

void MainWindow::viewKeyPress(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key::Key_Escape:
			refreshTools(Global::Tools::NoTool);
			break;
		case Qt::Key::Key_Enter:
			emit resetTool();
			break;
	}
}

void MainWindow::swapMode(int index)
{
	switch (index)
	{
		case Global::Mode::Object:
			ui->view2D->hide();
			ui->drawFrame->hide();

            ui->view3D->setFocus();
			ui->view3D->show();
            ui->leftFrame->show();
			break;
		case Global::Mode::Draw:
			ui->view2D->show();
			ui->drawFrame->show();

			ui->view3D->hide();
			ui->leftFrame->hide();
			break;
	}
}

void MainWindow::on_objectsTree_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->objectsTree->indexAt(pos);
    if (index.isValid()) {
		if(Item *item = dynamic_cast<Item*>(ui->objectsTree->itemAt(pos)))
        {
			QAction *selectedAction = objectContextMenu.exec(ui->objectsTree->viewport()->mapToGlobal(pos));
			if(selectedAction == &extrusionAction)
			{
				this->extrusionDialog->show(item);
			}
			else if(selectedAction == &redrawAction)
			{
				swapMode(Global::Mode::Draw);
				emit setTargetItem(item);
				this->ui->view2D->loadFromFile(item->getSketch());
			}
        }
		else if(Space::Plane *plane = dynamic_cast<Space::Plane*>(ui->objectsTree->itemAt(pos)))
        {
			QAction *selectedAction = planeContextMenu.exec(ui->objectsTree->viewport()->mapToGlobal(pos));
			if(selectedAction == &drawAction)
			{
				swapMode(Global::Mode::Draw);

				emit setTargetItem(plane);
			}
        }
    }
}

void MainWindow::on_objectsTree_itemSelectionChanged()
{
	this->ui->view3D->update();
}
