#include <QApplication>
#include <QtDebug>
#include <QMainWindow>

#include "MainWindow.h"
#include "networkTools.h"
#include "variables.h"

int main(int argc, char* argv[]) {
    qDebug() << "MR: Application started.";

    QApplication a(argc, argv);
    a.setApplicationDisplayName("MultiRelay");
    a.setApplicationName("MultiRelay");
    MainWindow w;
    w.show();

    return a.exec();
}
