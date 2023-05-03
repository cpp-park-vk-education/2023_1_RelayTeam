#include <QApplication>
#include <QtDebug>

#include "MainWindow.h"
// #include "PortScaner.h"
#include "networkTools.h"
#include "variables.h"

int main(int argc, char* argv[]) {
    qDebug() << "Application started.";
    //	QApplication a(argc, argv);
    //	a.setApplicationDisplayName("MultiRelay");
    //	a.setApplicationName("MultiRelay");
    //	MainWindow w;
    //	w.show();

    //	return a.exec();
    //    PortScaner Scaner;

    qDebug() << getPort(4000, 5000);
}
