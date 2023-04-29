#include "MainWindow.h"
#include "variables.h"
// #include <example_gstreamer.h>
#include <QApplication>
#include <QtDebug>

#include <ReciverVideo.h>
#include <SessionManager.h>
#include <TransmiterVideo.h>

int main(int argc, char *argv[])
{
    qDebug() << "Application123";
    QApplication a(argc, argv);

    //Reciver session1("A");
    ReciverVideo session1("127.0.0.1");
    TransmiterVideo session3("127.0.0.1", "0");

    //session3.start();
    //session1.start();

    qDebug() << "Application end.";
    //    a.setApplicationDisplayName("MultiRelay");
    //    a.setApplicationName("MultiRelay");
    //    MainWindow w;
    //    w.show();
    //    display(0, nullptr);

    return a.exec();
}
