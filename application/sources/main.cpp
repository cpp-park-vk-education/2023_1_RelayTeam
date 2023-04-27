#include "MainWindow.h"
#include "variables.h"
// #include <example_gstreamer.h>
#include <QApplication>
#include <QtDebug>


#include <reciver.h>
#include <transmmiter.h>
#include <example_gstreamer.h>

int main(int argc, char *argv[]) {
	qDebug() << "Application started.";
    QApplication a(argc, argv);

//    Reciver session1("A");
    Reciver session1("A"), session2("B");
    Transmiter session3("C"),session4("C");

//    display(NULL, NULL);


    session3.start();
    //    session4.start();

    session1.start();
//    session2.start();

    qDebug() << "Application end.";
//    a.setApplicationDisplayName("MultiRelay");
//    a.setApplicationName("MultiRelay");
//    MainWindow w;
//    w.show();
//    display(0, nullptr);

    return a.exec();
}
