#include "SslConnection.h"

#include <networkTools.h>

SslConnection::SslConnection(QSslKey key, QSslCertificate cert, QObject* parrent) : QThread(parrent) {
    this->start();
    this->moveToThread(this);
    QMetaObject::invokeMethod(this, "initSocket", Qt::BlockingQueuedConnection, Q_ARG(QSslKey, key), Q_ARG(QSslCertificate, cert));
}

void SslConnection::onConnectToClient(qintptr socket_descriptor) {
    ssl_socket->setSocketDescriptor(socket_descriptor);
    ssl_socket->startServerEncryption();
    qDebug() << "SSL Server detected incoming connection. ADDRESS: " << ssl_socket->peerAddress().toString();
    emit sendAddSslConnection(ssl_socket->peerAddress());
}

void SslConnection::onConnectToServer(QHostAddress address) {
    qDebug() << "SSL Client starts connection.";
    ssl_socket->connectToHostEncrypted(address.toString(), 12345);
    ssl_socket->setPeerVerifyMode(QSslSocket::VerifyNone);
    if (!ssl_socket->waitForEncrypted(30000)) {
        qDebug() << "SSL WAITING FOR ENCRYTION ERROR: " << ssl_socket->errorString();
        return;
    }
    emit sendAddSslConnection(ssl_socket->peerAddress());
}

void SslConnection::onWrite(QString message) {
    ssl_socket->write(message.toStdString().c_str());
}

bool SslConnection::isEncrypted() {
    return ssl_socket->isEncrypted() || ssl_socket->waitForEncrypted(30000);
}

void SslConnection::onSslErrors(const QList<QSslError>& errors) {
    foreach (const QSslError& error, errors) {
        qDebug() << "SSL ERROR: " << error.errorString();
    }
}

void SslConnection::onDisconnected() {
    qDebug("SSL : Client Disconnected");
    this->deleteLater();
}

void SslConnection::onReadyRead() {
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    QString message = clientSocket->readAll();
    qDebug() << message;
    QStringList message_items = message.split("|");
    if (message_items[0] == "mrelay-request-start-receiving-session") {
        QString session_type = message_items[1];
        QHostAddress local_ipv4_address = QHostAddress(clientSocket->peerAddress());
        qDebug() << "received request for receiving session from" << local_ipv4_address.toString()
                 << "\n session type is: " << session_type;
        emit sendStartReceivingSession(local_ipv4_address, session_type);
        return;
    }
    if (message_items[0] == "mrelay-ports-response") {  // prcesses port request response
        qint32 video_port = message_items[1].toInt();
        qint32 audio_port = message_items[2].toInt();
        QHostAddress localipv4_address = QHostAddress(clientSocket->peerAddress());
        qDebug() << "got port responce from: " << localipv4_address.toString() << "which contains video port: " << video_port
                 << " audio port:" << audio_port;
        emit sendReceivedPorts(localipv4_address, video_port, audio_port);
    }
    if (message_items[0] == "mrelay-request-initialization") {  // prcesses port request response
        QHostAddress other_localipv4_address = QHostAddress(clientSocket->peerAddress());
        qDebug() << "got initialization request: ";
        QString message = "mrelay-response-initialization|" + message_items[1] + "|" + getMacAddress();
        onWrite(message);
    }
    if (message_items[0] == "mrelay-response-initialization") {  // prcesses port request response
        QHostAddress other_localipv4_address = QHostAddress(clientSocket->peerAddress());
        qDebug() << "got initialization response from: " << clientSocket->peerAddress();
        emit sendInitializationResponse(message_items[1], message_items[2]);
    }
}

void SslConnection::initSocket(QSslKey key, QSslCertificate cert) {
    ssl_socket = new QSslSocket();
    connect(ssl_socket, &QSslSocket::readyRead, this, &SslConnection::onReadyRead);
    connect(ssl_socket, &QSslSocket::disconnected, this, &SslConnection::onDisconnected);
    connect(ssl_socket, qOverload<const QList<QSslError>&>(&QSslSocket::sslErrors), this,
            qOverload<const QList<QSslError>&>(&SslConnection::onSslErrors));
    ssl_socket->setProtocol(QSsl::AnyProtocol);
    ssl_socket->setPrivateKey(key);
    ssl_socket->setLocalCertificate(cert);
    ssl_socket->setPeerVerifyMode(QSslSocket::VerifyNone);
}
