#include "MainWindow.h"
#include "variables.h"
// #include <example_gstreamer.h>
#include <QApplication>
#include <QtDebug>

#include <ReciverVideo.h>
#include <SessionManager.h>
#include <TransmiterVideo.h>
#include <example_gstreamer.h>

int main(int argc, char *argv[])
{
    qDebug() << "Application123";
    QApplication a(argc, argv);

    //Reciver session1("A");
    ReciverVideo session1("A");
    TransmiterVideo session3("C");

    //    display(NULL, NULL);

    // session4.start();
    session3.start();
    session3.quit();
    //    session1.start_reciver();
    //session1.start();

    //session1.wait();

    // session1.quit();
    //    session1.wait();

    qDebug() << "Application end.";
    //    a.setApplicationDisplayName("MultiRelay");
    //    a.setApplicationName("MultiRelay");
    //    MainWindow w;
    //    w.show();
    //    display(0, nullptr);

    return a.exec();
}
