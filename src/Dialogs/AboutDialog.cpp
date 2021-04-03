#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
	delete ui;
}

void AboutDialog::on_pushButton_clicked()
{
	QDesktopServices::openUrl(QUrl("https://github.com/CGAL/cgal/blob/master/Installation/LICENSE"));
}

void AboutDialog::on_pushButton_2_clicked()
{
	QDesktopServices::openUrl(QUrl("https://github.com/nlohmann/json/blob/develop/LICENSE.MIT"));
}

void AboutDialog::on_pushButton_3_clicked()
{
	QDesktopServices::openUrl(QUrl("https://github.com/delfrrr/delaunator-cpp/blob/master/LICENSE"));
}

void AboutDialog::on_pushButton_4_clicked()
{
	QDesktopServices::openUrl(QUrl("https://www.gnu.org/licenses/lgpl-3.0-standalone.html"));
}
