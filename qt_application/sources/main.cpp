#include "MainWindow.h"
#include "variables.h"
#include <example_gstreamer.h>
#include <QApplication>
#include <QtDebug>

int main(int argc, char *argv[]) {
	qDebug() << "Application started.";
	QApplication a(argc, argv);
	a.setApplicationDisplayName("MultiRelay");
	a.setApplicationName("MultiRelay");
	MainWindow w;
	w.show();
	//	display(0, nullptr);

	return a.exec();
}
