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

    void sendScreenshot();

public:
    explicit BluetoothClient(QObject* parent = nullptr);

    ~BluetoothClient();

private slots:
    void onReadSocket();

    void onConnected();

    void onDisconnected();

    void onSocketErrorOccurred(QBluetoothSocket::SocketError);

public slots:
    void connectToService(const QBluetoothServiceInfo& remote_service);

    void killConnection();

    void onStartSendingScreenshots();

    void onSendMessage(const QString& message);

signals:
    void sendMessageReceived(const QString& sender, const QString& message);

    void sendDisconnected();
};
