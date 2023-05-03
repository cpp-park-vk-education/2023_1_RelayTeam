#pragma once
#include <QDebug>
#include <QObject>
#include <QString>
#include <QTcpSocket>

class PortScaner : public QObject {
    Q_OBJECT
public:
    qint32 getPort();

private:
    bool portIsBusy(QString ip, qint32 port);
};
