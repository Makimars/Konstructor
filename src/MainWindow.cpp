#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	loadSettings();

	this->ui->setupUi(this);

	this->settingsDialog = new SettingsDialog();
	this->extrusionDialog = new ExtrusionDialog();
	MessagesManager::init();

	this->setupUi();
	this->setupConnections();
}

MainWindow::~MainWindow()
{
	delete this->ui;
}

void MainWindow::show()
{
	//welcome screen
	WelcomeDialog *welcomeDialog = new WelcomeDialog(this);
	welcomeDialog->exec();

	switch (welcomeDialog->action)
	{
		case WelcomeScreenResult::NewProject:
			//new Project
			emit setTargetItem(this->ui->objectsTree->topLevelItem(0));
			this->ui->objectsTree->setCurrentItem(this->ui->objectsTree->topLevelItem(0));
			this->setMode(Global::Mode::Draw);
			break;
		case WelcomeScreenResult::OpenProject:
			//open Project
			on_openObjectFile_clicked();
			this->setMode(Global::Mode::Object);
			break;
		case WelcomeScreenResult::About:
			break;
		case WelcomeScreenResult::Exit:
		default:
			this->close();
			this->hide();
			QApplication::closeAllWindows();
			QApplication::quit();
			break;
	}

	QMainWindow::show();
}

void MainWindow::setupUi()
{
    //object menu
	this->ui->objectsTree->setHeaderHidden(true);
    ui->objectsTree->setContextMenuPolicy(Qt::CustomContextMenu);

	Plane *basePlane = new Plane();
	basePlane->setPosition(QVector3D(0,0,0));
	basePlane->setRotation(QQuaternion(1,0,0,0));
	basePlane->setText(0, "origin plane");
    this->ui->objectsTree->addTopLevelItem(basePlane);
	this->ui->view3D->setTopPlane(basePlane);

    //context menus
	drawAction.setText(tr("Draw"));
    planeContextMenu.addAction(&drawAction);

	extrusionAction.setText(tr("Extrude"));
	objectContextMenu.addAction(&extrusionAction);
	redrawAction.setText(tr("Redraw"));
	objectContextMenu.addAction(&redrawAction);
	deleteAction.setText(tr("Delete"));
	objectContextMenu.addAction(&deleteAction);
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
			SpaceFactory::getInstance(), &SpaceFactory::recieveTargetItem
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
			SpaceFactory::getInstance(), &SpaceFactory::addNewItem
			);

	//connect extrusionDialog to screen update
	connect(extrusionDialog, &ExtrusionDialog::selectionChanged,
			this->ui->view3D, &View3DWidget::update
			);

	//generating drawables for loading items
	connect(SpaceFactory::getInstance(), &SpaceFactory::generatePolygons,
			this, &MainWindow::getPolygonsForItem
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
	if(tool != Global::Tools::ArcTool)
		this->ui->arcButton->setChecked(false);

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
	this->ui->view3D->reset();
}

void MainWindow::on_openObjectFile_clicked()
{
	this->ui->view3D->reset();
	QString fileName = QFileDialog::getOpenFileName(
			this,
			Global::openFile,
			Settings::userProjectRoot,
			Global::konstructorProject + ";;" + Global::allFiles
			);

	QFile file(fileName);
	if(file.exists())
	{
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
			this->ui->view3D->loadFromFile(file.readAll());
	}
}

void MainWindow::on_saveObjectButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(
			this,
			Global::saveFile,
			Settings::userProjectRoot,
			Global::konstructorProject + ";;" + Global::allFiles
			);
	this->ui->view3D->saveToFile(fileName);
}

void MainWindow::on_exportObjectButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(
			this,
			Global::saveFile,
			Settings::userProjectRoot,
			"STL (*.stl)"
			);
	this->ui->view3D->exportToFile(fileName);
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
	setMode(Global::Mode::Object);
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

void MainWindow::on_arcButton_clicked()
{
	if(this->ui->arcButton->isChecked())
		refreshTools(Global::Tools::ArcTool);
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

	setMode(Global::Mode::Object);
	emit finishDrawing();
}

//-----    misc slots    -----

void MainWindow::on_objectsTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	setMode(Global::Mode::Draw);
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

void MainWindow::setMode(int index)
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
				item->setSelected(false);
				this->extrusionDialog->show(item);
			}
			else if(selectedAction == &redrawAction)
			{
				setMode(Global::Mode::Draw);
				emit setTargetItem(item);
				this->ui->view2D->loadFromFile(item->getSketch());
			}
			else if(selectedAction == &deleteAction)
			{
				SpaceFactory::getInstance()->deleteItem(item);
			}
        }
		else if(Plane *plane = dynamic_cast<Plane*>(ui->objectsTree->itemAt(pos)))
        {
			QAction *selectedAction = planeContextMenu.exec(ui->objectsTree->viewport()->mapToGlobal(pos));
			if(selectedAction == &drawAction)
			{
				setMode(Global::Mode::Draw);

				emit setTargetItem(plane);
			}
        }
    }

	this->ui->view3D->update();
}

void MainWindow::on_objectsTree_itemClicked(QTreeWidgetItem *item, int column)
{
	this->ui->view3D->update();
}

std::vector<QPolygonF> MainWindow::getPolygonsForItem(QString sketch)
{
	QVector<DrawableObject*> loadedObjects = Factory::getInstance()->generateListFromSketch(sketch);

	return Polygonator::getInstance()->generatePolygons(loadedObjects);
}
