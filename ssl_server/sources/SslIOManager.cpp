#include "SslIOManager.h"

SslIOManager::SslIOManager(QObject* parent) : QObject(parent) {
    ServerThread* ssl_server_thread = new ServerThread();
    QMetaObject::invokeMethod(ssl_server_thread, "init", Qt::BlockingQueuedConnection, Q_ARG(SslIOManager*, this));
    ssl_server = reinterpret_cast<SslServer*>(ssl_server_thread->getObject());

    connect(this, &SslIOManager::sendKillAll, ssl_server_thread, &ServerThread::deleteLater);
}

SslIOManager::~SslIOManager() {
    emit sendKillAll();
}

void SslIOManager::onAddSslConnection(QHostAddress address) {
    connections_map[address] = reinterpret_cast<SslConnection*>(sender());
}

void SslIOManager::onRemoveSslConnection(QHostAddress address) {
    connections_map.remove(address);
}

void SslIOManager::writeTo(QHostAddress address, QString message) {
    if (!connections_map.contains(address)) {
        SslConnection* ssl_connection = nullptr;
        QMetaObject::invokeMethod(ssl_server, "onConnectToServer", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(SslConnection*, ssl_connection), Q_ARG(QHostAddress, address));
        bool isEncrypted = false;
        QMetaObject::invokeMethod(ssl_connection, "isEncrypted", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, isEncrypted));
        if (!isEncrypted) {
            qDebug() << "SSL: Unable to write";
            return;
        }
        QMetaObject::invokeMethod(ssl_connection, "onWrite", Qt::QueuedConnection, Q_ARG(QString, message));
    } else {
        QMetaObject::invokeMethod(connections_map[address], "onWrite", Qt::QueuedConnection, Q_ARG(QString, message));
    }
}
/////////////////////////INDIVIDUAL REQUESTS/////////////////////////////////////////
void SslIOManager::onSendPorts(const QHostAddress ip_address, qint32 video_port, qint32 audio_port) {
    qDebug() << "sending ports: " << video_port << "  " << audio_port << "to: " << ip_address;
    QString message = tr("mrelay-ports-response|") + QString::number(video_port) + tr("|") + QString::number(audio_port);
    writeTo(ip_address, message);
}

void SslIOManager::onStartReciver(const QHostAddress ip_address, const QString session_type) {
    qDebug() << "sending receive request to: " << ip_address.toString();
    QString message = "mrelay-request-start-receiving-session|" + session_type.toUtf8();
    writeTo(ip_address, message);
}

void SslIOManager::onRequestInitialization(const QHostAddress ip_address, QString service_name) {
    qDebug() << "sending receive request to: " << ip_address.toString();
    QString message = "mrelay-request-initialization|" + service_name;
    writeTo(ip_address, message);
}
