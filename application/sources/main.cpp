#include <QApplication>
#include <QtDebug>

#include "MainWindow.h"
#include "SessionManager.h"
#include "variables.h"

int main(int argc, char* argv[]) {
    qDebug() << "Application started.";
    //	QApplication a(argc, argv);
    //	a.setApplicationDisplayName("MultiRelay");
    //	a.setApplicationName("MultiRelay");
    //	MainWindow w;
    //	w.show();

    //	return a.exec();
    SessionManager Ses;
    //    127.0.0.54:53
    qDebug() << Ses.portIsBusy("127.0.0.54", 53);
    qDebug() << Ses.portIsBusy("localhost", 631);
    qDebug() << Ses.getPort();
}
