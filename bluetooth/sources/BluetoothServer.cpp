#include "BluetoothServer.h"

#include <QtBluetooth/QBluetoothDeviceInfo>

#include "variables.h"

BluetoothServer::BluetoothServer(QObject* parent) : QObject(parent) {}

BluetoothServer::~BluetoothServer() {
    service_info.unregisterService();

    qDeleteAll(server_sockets);
}

void BluetoothServer::initServer(const QBluetoothAddress& localAdapter) {
    qDebug() << "Initiolizing bluetooth server";
    rfcomm_server = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcomm_server, &QBluetoothServer::newConnection, this, &BluetoothServer::onClientConnected);
    bool result = rfcomm_server->listen(localAdapter);
    if (!result) {
        qWarning() << "Bluetooth server: Cannot bind server to" << localAdapter.toString();
        return;
    }
    QBluetoothServiceInfo::Sequence profileSequence;
    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));
    classId << QVariant::fromValue(quint16(0x100));
    profileSequence.append(QVariant::fromValue(classId));
    service_info.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList, profileSequence);

    classId.clear();
    classId << QVariant::fromValue(QBluetoothUuid(serviceUuid));
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));

    service_info.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    name = QString(service_info.device().name() + "server");
    service_info.setAttribute(QBluetoothServiceInfo::ServiceName, tr(name.toStdString().c_str()));
    service_info.setAttribute(QBluetoothServiceInfo::ServiceDescription, tr("bluetooth server"));
    service_info.setAttribute(QBluetoothServiceInfo::ServiceProvider, true);

    service_info.setServiceUuid(QBluetoothUuid(serviceUuid));

    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::PublicBrowseGroup));
    service_info.setAttribute(QBluetoothServiceInfo::BrowseGroupList, publicBrowse);

    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ProtocolUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ProtocolUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcomm_server->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    service_info.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList, protocolDescriptorList);

    service_info.registerService(localAdapter);
}

void BluetoothServer::onSetServiceName(QString _name) {
    name = _name;
    service_info.setAttribute(QBluetoothServiceInfo::ServiceName, tr(name.toStdString().c_str()));
}

void BluetoothServer::onSendMessage(const QString& message) {
    QByteArray text = message.toUtf8();

    for (QBluetoothSocket* socket : qAsConst(server_sockets)) {
        socket->write(text);
    }
}

void BluetoothServer::onClientConnected() {
    QBluetoothSocket* socket = rfcomm_server->nextPendingConnection();
    if (!socket) {
        return;
    }

    connect(socket, &QBluetoothSocket::readyRead, this, &BluetoothServer::onReadSocket);
    connect(socket, &QBluetoothSocket::disconnected, this, &BluetoothServer::onClientDisconnected);

    QString peerAddress = socket->peerAddress().toString();

    server_sockets.insert(peerAddress, socket);
    qDebug() << "Bluetooth server: Client " << socket->peerName() << " connected.";
}

void BluetoothServer::onClientDisconnected() {
    QBluetoothSocket* socket = qobject_cast<QBluetoothSocket*>(sender());
    if (!socket) {
        return;
    }
    qDebug() << "Bluetooth server: Client " << socket->peerName() << " disconnected.";

    server_sockets.remove(socket->peerAddress().toString());

    socket->deleteLater();
}

void BluetoothServer::onReadSocket() {
    QBluetoothSocket* socket = qobject_cast<QBluetoothSocket*>(sender());
    if (!socket) {
        return;
    }

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        emit sendMessageReceived(socket->peerName(), QString::fromUtf8(line.constData(), line.length()));
    }
}
