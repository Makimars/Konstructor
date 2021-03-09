#include "Dialogs/WelcomeDialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow m;
	WelcomeDialog d;
	d.show(&m);

	return a.exec();
}
