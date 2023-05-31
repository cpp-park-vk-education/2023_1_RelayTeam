#include "server.h"

#include <QtBluetooth/qbluetoothserver.h>
#include <QtBluetooth/qbluetoothsocket.h>

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

Server::Server(QObject *parent)
    :   QObject(parent) {
}

Server::~Server() {
    stopServer();
}

void Server::startServer(const QBluetoothAddress& localAdapter) {
    if (rfcomm_server) {
        return;
    }

    rfcomm_server = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcomm_server, &QBluetoothServer::newConnection,
            this, QOverload<>::of(&Server::onClientConnected));
    bool result = rfcomm_server->listen(localAdapter);
    if (!result) {
        qWarning() << "Cannot bind server to" << localAdapter.toString();
        return;
    }

    QBluetoothServiceInfo::Sequence profileSequence;
    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    classId << QVariant::fromValue(quint16(0x100));
    profileSequence.append(QVariant::fromValue(classId));
    service_info.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                             profileSequence);

    classId.clear();
    classId << QVariant::fromValue(QBluetoothUuid(serviceUuid));
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));

    service_info.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    name = QString(service_info.device().name() + "server");
    service_info.setAttribute(QBluetoothServiceInfo::ServiceName, tr(name.toStdString().c_str()));
    service_info.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("bluetooth server"));
    service_info.setAttribute(QBluetoothServiceInfo::ServiceProvider, true);

    service_info.setServiceUuid(QBluetoothUuid(serviceUuid));

    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    service_info.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                             publicBrowse);

    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcomm_server->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    service_info.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);

    service_info.registerService(localAdapter);
}

void Server::stopServer() {
    service_info.unregisterService();

    qDeleteAll(client_sockets);

    delete rfcomm_server;
    rfcomm_server = nullptr;
}

void Server::onRename(QString _name) {
    name = _name;   
    service_info.setAttribute(QBluetoothServiceInfo::ServiceName, tr(name.toStdString().c_str()));
}

void Server::onSendMessage(const QString &message) {
    QByteArray text = message.toUtf8();

    for (QBluetoothSocket *socket : qAsConst(client_sockets)){
        socket->write(text);
    }
}

void Server::onSendMacAndName(const QString &mac, const QString &name)
{
    emit sendMacAndName(mac, name);
}

void Server::onClientConnected() {
    QBluetoothSocket *socket = rfcomm_server->nextPendingConnection();
    if (!socket){
        return;
    }

    connect(socket, &QBluetoothSocket::readyRead, this, &Server::onReadSocket);
    connect(socket, &QBluetoothSocket::disconnected, this, QOverload<>::of(&Server::onClientDisconnected));
    QString peerAddress = socket->peerAddress().toString();
    client_sockets.insert(peerAddress, socket);
    emit sendClientConnected(socket->peerName());
}

void Server::onClientDisconnected() {
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket) {
        return;
    }

    emit sendClientDisconnected(socket->peerName());

    client_sockets.remove(socket->peerAddress().toString());

    socket->deleteLater();
}

void Server::onReadSocket() {
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket) {
        return;
    }

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        emit sendMessageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}
