#pragma once
#include <QDebug>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <cassert>

class PortScaner : public QObject {
    Q_OBJECT
public:
    qint32 getPort(qint32 startRangeSearch = 4000, qint32 stopRangeSearch = 5000, QString ip = "localhost");

private:
    bool portIsBusy(QString ip, qint32 port);
};
