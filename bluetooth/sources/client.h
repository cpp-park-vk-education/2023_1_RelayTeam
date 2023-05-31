#ifndef CLIENT_H
#define CLIENT_H

#include "bluetoothformatimage.h"

#include <QtCore/qobject.h>

#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothsocket.h>

QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    void startClient(const QBluetoothServiceInfo &remote_service);
    void stopClient();

public slots:
    void onSendMessage(const QString &message);

signals:
    void sendMessageReceived(const QString &sender, const QString &message);
    void sendConnected(const QString &name);
    void sendDisconnected();
    void sendSocketErrorOccurred(const QString &error_string);

private slots:
    void onReadSocket();
    void onConnected();
    void onSocketErrorOccurred(QBluetoothSocket::SocketError);

private:
    QBluetoothSocket *socket = nullptr;
};
#endif // CLIENT_H
