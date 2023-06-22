#include "SSLServer.h"

#include <QFile>
#include <QMetaType>

#include "keygen.h"

SslServer::SslServer(SslIOManager* ssl_io_manager_, QObject* parent) : ssl_io_manager(ssl_io_manager_), QTcpServer(parent) {
    qRegisterMetaType<qintptr>("qintptr");
    QPair<QSslKey, QSslCertificate> key_pair = generateKeyPair();
    key = key_pair.first;
    cert = key_pair.second;
    if (!listen(QHostAddress::Any, 12345)) {
        qCritical() << "SSL Unable to start the TCP server:" << errorString();
    }
}

SslServer::~SslServer() {
    emit sendKillAll();
}

void SslServer::connectSslConnection(SslConnection* ssl_connection) {
    connect(ssl_connection, &SslConnection::sendStartReceivingSession, ssl_io_manager, &SslIOManager::sendStartReceivingSession);
    connect(ssl_connection, &SslConnection::sendReceivedPorts, ssl_io_manager, &SslIOManager::sendReceivedPorts);
    connect(ssl_connection, &SslConnection::sendInitializationResponse, ssl_io_manager, &SslIOManager::sendInitializationResponse);

    connect(ssl_connection, &SslConnection::sendAddSslConnection, ssl_io_manager, &SslIOManager::onAddSslConnection);
    connect(ssl_connection, &SslConnection::sendRemoveSslConnection, ssl_io_manager, &SslIOManager::onRemoveSslConnection);
}

void SslServer::incomingConnection(qintptr socketDescriptor) {
    SslConnection* ssl_connection = new SslConnection(key, cert);
    connectSslConnection(ssl_connection);

    connect(this, &SslServer::sendKillAll, ssl_connection, &SslConnection::deleteLater);

    QMetaObject::invokeMethod(ssl_connection, "onConnectToClient", Qt::QueuedConnection, Q_ARG(qintptr, socketDescriptor));
}

SslConnection* SslServer::onConnectToServer(QHostAddress address) {
    SslConnection* ssl_connection = new SslConnection(key, cert);
    connectSslConnection(ssl_connection);

    connect(this, &SslServer::sendKillAll, ssl_connection, &SslConnection::deleteLater);

    QMetaObject::invokeMethod(ssl_connection, "onConnectToServer", Qt::QueuedConnection, Q_ARG(QHostAddress, address));
    return ssl_connection;
}
