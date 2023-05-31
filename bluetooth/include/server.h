#pragma once

#include <QObject>
#include <QMap>

#include "bluetoothformatimage.h"

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>

QT_FORWARD_DECLARE_CLASS(QBluetoothServer)
QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

class Server : public QObject {
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    void startServer(const QBluetoothAddress &local_adapter = QBluetoothAddress());
    void stopServer();

public slots:
    void onRename(QString name);
    void onSendMessage(const QString &message);
    void onSendMacAndName(const QString &mac, const QString &name);

signals:
    void sendMacAndName(const QString &mac, const QString &name);
    void sendMessageReceived(const QString &sender, const QString &message);
    void sendClientConnected(const QString &name);
    void sendClientDisconnected(const QString &name);

private slots:
    void onClientConnected();
    void onClientDisconnected();
    void onReadSocket();

private:
    QString name;
    QBluetoothServer *rfcomm_server = nullptr;
    QBluetoothServiceInfo service_info;
    QMap<QString, QBluetoothSocket*> client_sockets;
};
