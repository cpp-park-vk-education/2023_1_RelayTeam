#pragma once

#include <variables.h>

#include <QSslCertificate>
#include <QSslKey>
#include <QTcpServer>
#include <QThread>

#include "SslConnection.h"
#include "SslIOManager.h"

class SslIOManager;

class SslServer : public QTcpServer {
private:
    Q_OBJECT

    QSslKey key;
    QSslCertificate cert;
    SslIOManager* ssl_io_manager;

    void connectSslConnection(SslConnection* ssl_connection);

protected:
    void incomingConnection(qintptr socketDescriptor);

public:
    SslServer(SslIOManager* ssl_io_manager_, QObject* parent = nullptr);

    ~SslServer();

public slots:
    SslConnection* onConnectToServer(QHostAddress address);

signals:
    void sendKillAll();
};

class ServerThread : public QThread {
private:
    Q_OBJECT

    QObject* server_object;

public:
    ServerThread(QObject* parent = nullptr) : QThread(parent), server_object(nullptr) {
        this->moveToThread(this);
        this->start();
    }

    QObject* getObject() {
        return reinterpret_cast<QObject*>(server_object);
    }

public slots:
    void init(SslIOManager* ssl_io_manager) {
        server_object = new SslServer(ssl_io_manager);
        server_object->moveToThread(this);
    }
};
