#include "client.h"

#include <QtCore/qmetaobject.h>

Client::Client(QObject *parent)
    :   QObject(parent) {}

Client::~Client() {
    stopClient();
}

void Client::startClient(const QBluetoothServiceInfo &remote_service) {
    if (socket){
        return;
    }

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    socket->connectToService(remote_service);
    qDebug() << "ConnectToService done";

    connect(socket, &QBluetoothSocket::readyRead, this, &Client::onReadSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&Client::onConnected));
    connect(socket, &QBluetoothSocket::disconnected, this, &Client::sendDisconnected);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &Client::onSocketErrorOccurred);
}

void Client::stopClient() {
    delete socket;
    socket = nullptr;
}

void Client::onReadSocket() {
    if (!socket) {
        return;
    }

    while (socket->canReadLine()) {
        QByteArray line = socket->readAll().trimmed();
        emit sendMessageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}

void Client::onSendMessage(const QString &message) {
    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);
}

void Client::onSocketErrorOccurred(QBluetoothSocket::SocketError error) {
    if (error == QBluetoothSocket::NoSocketError) {
        return;
    }

    QMetaEnum metaEnum = QMetaEnum::fromType<QBluetoothSocket::SocketError>();
    QString error_string = socket->peerName() + QLatin1Char(' ')
                          + metaEnum.valueToKey(error) + QLatin1String(" occurred");

    emit sendSocketErrorOccurred(error_string);
}

void Client::onConnected() {
    emit sendConnected(socket->peerName());
}
