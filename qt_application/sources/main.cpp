#include <QApplication>
#include "mainwindow.h"
#include <example_gstreamer.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    display(0 , nullptr);


    return a.exec();
}
