#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	loadSettings();

	this->ui->setupUi(this);
	this->setupUi();

	this->settingsDialog = new SettingsDialog();
	this->extrusionDialog = new ExtrusionDialog();
	MessagesManager::init();

	this->swapMode(Global::Mode::Object);

	connect(this->ui->view2D, &ViewWidget::keyPressed,
			this, &MainWindow::viewKeyPress
			);
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

	//drawing to Space connection
	connect(this, &MainWindow::setTargetItem,
			this->ui->view3D, &View3DWidget::recieveTargetItem
			);
	connect(this, &MainWindow::finishDrawing,
			this->ui->view2D, &ViewWidget::finishDrawing
			);
	connect(this->ui->view2D, &ViewWidget::returnDrawing,
			Polygonator::getInstance(), &Polygonator::recieveDrawing
			);
	connect(Polygonator::getInstance(), &Polygonator::sendPolygons,
			this->ui->view3D, &View3DWidget::addItem
			);
}

MainWindow::~MainWindow()
{
	delete this->ui;
}

void MainWindow::setupUi()
{
    //menubar
    QMenuBar *menuBar = new QMenuBar(this->ui->leftFrame);
	QMenu *fileMenu = menuBar->addMenu(tr("File"));
    this->ui->leftFrame->layout()->setContentsMargins(4,menuBar->height(),4,4);

	QAction *newAction = fileMenu->addAction(tr("New"));
	connect(newAction, &QAction::triggered,
			this, &MainWindow::newFileClicked
			);
	QAction *openAction = fileMenu->addAction(tr("Open"));
	connect(openAction, &QAction::triggered,
            this, &MainWindow::openFileClicked
            );
	QAction *saveAction = fileMenu->addAction(tr("Save"));
	connect(saveAction, &QAction::triggered,
            this, &MainWindow::saveFileClicked
            );
	QAction *exportAction = fileMenu->addAction(tr("Export"));
	connect(exportAction, &QAction::triggered,
            this, &MainWindow::exportFileClicked
            );
	QAction *settingsAction = fileMenu->addAction(tr("Settings"));
	connect(settingsAction, &QAction::triggered,
            this, &MainWindow::settingsClicked
            );
	QAction *QuitAction = fileMenu->addAction(tr("Quit"));
	connect(QuitAction, &QAction::triggered,
            this, &MainWindow::quitClicked
            );

	newAction->setShortcut(Settings::newFile);
	openAction->setShortcut(Settings::openFile);
	saveAction->setShortcut(Settings::saveFile);
	exportAction->setShortcut(Settings::exportFile);
	settingsAction->setShortcut(Settings::openSettings);
	QuitAction->setShortcut(Settings::quitApp);

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


void MainWindow::newFileClicked()
{

}

void MainWindow::openFileClicked()
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

void MainWindow::saveFileClicked()
{
	QString fileName = QFileDialog::getSaveFileName(
			this,
			Global::saveFile,
			Settings::userProjectRoot,
			Global::konstructorSketch + ";;" + Global::allFiles
			);
	this->ui->view2D->saveToFile(fileName);
}

void MainWindow::exportFileClicked()
{

}

void MainWindow::settingsClicked()
{
	this->settingsDialog->exec();
}

void MainWindow::quitClicked()
{
	this->close();
}

//-----    draw tab    -----

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

void MainWindow::on_dimensionButton_clicked()
{
	if(this->ui->dimensionButton->isChecked())
		refreshTools(Global::Tools::DimensionTool);
	else
		refreshTools(Global::Tools::NoTool);
}

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

void MainWindow::on_importSketchButton_clicked()
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
