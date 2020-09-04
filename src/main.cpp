#include "JobshopGUI.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	JobshopGUI w;
	w.show();
	return a.exec();
}

