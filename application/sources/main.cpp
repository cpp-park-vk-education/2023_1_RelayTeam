#include <QApplication>
#include <QMainWindow>
#include <QtDebug>

#include "MainWindow.h"

int main(int argc, char* argv[]) {
    qDebug() << "MR: Application started.";

    QApplication application(argc, argv);
    application.setApplicationDisplayName("MultiRelay");
    application.setApplicationName("MultiRelay");
    MainWindow main_window(qApp->primaryScreen());
    main_window.show();

    return application.exec();
}
