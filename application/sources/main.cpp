#include <QApplication>
#include <QtDebug>

#include "MainWindow.h"
#include "PortScaner.h"
#include "variables.h"

int main(int argc, char* argv[]) {
    qDebug() << "Application started.";
    //	QApplication a(argc, argv);
    //	a.setApplicationDisplayName("MultiRelay");
    //	a.setApplicationName("MultiRelay");
    //	MainWindow w;
    //	w.show();

    //	return a.exec();
    PortScaner Scaner;
    //    127.0.0.54:53
    //    qDebug() << Scaner.portIsBusy("127.0.0.54", 53);
    //    qDebug() << Scaner.portIsBusy("localhost", 631);
    qDebug() << Scaner.getPort();
}
