#pragma once

#include <QObject>
#include <QScreen>
#include <QtBluetooth/QBluetoothServiceInfo>
#include <QtBluetooth/QBluetoothSocket>
#include <QtGui>

class BluetoothClient : public QObject {
private:
    Q_OBJECT

    QBluetoothSocket* bluetooth_client_socket = nullptr;

    void onSendStarted();

    void captureScreen();

public:
    explicit BluetoothClient(QObject* parent = nullptr);

    ~BluetoothClient();

    void connectToService(const QBluetoothServiceInfo& remote_service);

    void killConnection();

private slots:
    void onReadSocket();

    void onConnected();

    void onDisconnected();

    void onSocketErrorOccurred(QBluetoothSocket::SocketError);

public slots:
    void onSendMessage(const QString& message);

signals:
    void sendMessageReceived(const QString& sender, const QString& message);

    void sendDisconnected();
};
