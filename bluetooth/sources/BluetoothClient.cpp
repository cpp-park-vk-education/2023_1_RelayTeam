#include "BluetoothClient.h"

#include <QMetaEnum>
#include <QMetaObject>

BluetoothClient::BluetoothClient(QObject* parent) : QObject(parent) {}

BluetoothClient::~BluetoothClient() {
    killConnection();
}

void BluetoothClient::connectToService(const QBluetoothServiceInfo& remote_service) {
    if (bluetooth_client_socket) {
        return;
    }

    // Connect to service
    bluetooth_client_socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    bluetooth_client_socket->connectToService(remote_service);
    qDebug() << "ConnectToService done";

    connect(bluetooth_client_socket, &QBluetoothSocket::readyRead, this, &BluetoothClient::onReadSocket);
    connect(bluetooth_client_socket, &QBluetoothSocket::connected, this, &BluetoothClient::onConnected);
    connect(bluetooth_client_socket, &QBluetoothSocket::disconnected, this, &BluetoothClient::onDisconnected);
    connect(bluetooth_client_socket, &QBluetoothSocket::errorOccurred, this, &BluetoothClient::onSocketErrorOccurred);
}

void BluetoothClient::killConnection() {
    if (!bluetooth_client_socket) {
        return;
    }
    bluetooth_client_socket->disconnect();
    bluetooth_client_socket->deleteLater();
    bluetooth_client_socket = nullptr;
}

void BluetoothClient::onConnected() {
    qDebug() << "Bluetooth client socket: " << bluetooth_client_socket->peerName() << " CONNECTED.";
}

void BluetoothClient::onDisconnected() {
    qDebug() << "Bluetooth client socket: " << bluetooth_client_socket->peerName() << " DISCONNECTED.";
}

void BluetoothClient::onSocketErrorOccurred(QBluetoothSocket::SocketError error) {
    if (error == QBluetoothSocket::SocketError::NoSocketError) {
        return;
    }
    QMetaEnum meta_enum = QMetaEnum::fromType<QBluetoothSocket::SocketError>();

    qDebug() << "Bluetooth client socket error: "
             << bluetooth_client_socket->peerName() + QString(": ") + QString::fromUtf8(meta_enum.valueToKey(static_cast<int>(error)));
}

void BluetoothClient::onReadSocket() {
    if (!bluetooth_client_socket) {
        return;
    }

    while (bluetooth_client_socket->canReadLine()) {
        QByteArray line = bluetooth_client_socket->readAll().trimmed();
        emit sendMessageReceived(bluetooth_client_socket->peerName(), QString::fromUtf8(line.constData(), line.length()));
    }
}

void BluetoothClient::onSendMessage(const QString& message) {
    QByteArray text = message.toUtf8() + '\n';
    bluetooth_client_socket->write(text);
}

void BluetoothClient::onStartSendingScreenshots() {
    QEventLoop loop;
    while (true) {
        loop.processEvents();
        sendScreenshot();
    }
}

void BluetoothClient::sendScreenshot() {
    QScreen* screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);

    QByteArray data_image;
    QBuffer buffer(&data_image);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    QByteArray image_byte_array = data_image;
    QString message = image_byte_array.toBase64() + '\n';
    onSendMessage(message);
}
