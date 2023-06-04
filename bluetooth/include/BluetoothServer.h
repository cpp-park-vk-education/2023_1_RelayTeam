#pragma once

#include <QMap>
#include <QObject>
#include <QtBluetooth/QBluetoothAddress>
#include <QtBluetooth/QBluetoothServer>
#include <QtBluetooth/QBluetoothServiceInfo>
#include <QtBluetooth/QBluetoothSocket>

class BluetoothServer : public QObject {
private:
    Q_OBJECT

    QString name;
    QBluetoothServer* rfcomm_server = nullptr;
    QBluetoothServiceInfo service_info;
    QMap<QString, QBluetoothSocket*> server_sockets;

public:
    explicit BluetoothServer(QObject* parent = nullptr);

    ~BluetoothServer();

    void stopServer();

private slots:
    void onClientConnected();

    void onClientDisconnected();

    void onReadSocket();

public slots:
    void initServer(const QBluetoothAddress& local_adapter = QBluetoothAddress());

    void onSetServiceName(QString name);

    void onSendMessage(const QString& message);

signals:
    void sendMessageReceived(const QString& sender, const QString& message);
};
