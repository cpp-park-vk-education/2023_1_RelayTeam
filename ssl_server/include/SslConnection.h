#pragma once

#include <QSslCertificate>
#include <QSslKey>
#include <QSslSocket>
#include <QThread>

class SslConnection : public QThread {
private:
    Q_OBJECT

public:
    QSslSocket* ssl_socket;

    /*!
     * \brief Moves object to related thread for later QSslSocket initialization and execution.
     * \param key - private key of server.
     * \param cert - public key of server.
     * \param parrent
     */
    SslConnection(QSslKey key, QSslCertificate cert, QObject* parrent = nullptr);

    virtual ~SslConnection() override {
        emit sendRemoveSslConnection(ssl_socket->peerAddress());
        ssl_socket->deleteLater();
    }

public slots:
    /*!
     * \brief Creates server-side connection with QSslSocket.
     * \param socket_descriptor
     */
    void onConnectToClient(qintptr socket_descriptor);

    /*!
     * \brief Creates client-side connection with QSslSocket.
     * \param address - address of server.
     */
    void onConnectToServer(QHostAddress address);

    /*!
     * \brief Writes to opened QSslSocket.
     * \param message - text of socket message.
     */
    void onWrite(QString message);

    bool isEncrypted();

private slots:
    void onSslErrors(const QList<QSslError>& errors);

    void onDisconnected();

    void onReadyRead();

    /*!
     * \brief Creates QSslSocket in related thread.
     * \param key
     * \param cert
     */
    void initSocket(QSslKey key, QSslCertificate cert);

signals:
    void sendAddSslConnection(QHostAddress address);

    void sendRemoveSslConnection(QHostAddress address);

    void sendStartReceivingSession(QHostAddress ip_address, QString session_type);

    void sendReceivedPorts(const QHostAddress ipv6_address, qint32 video_port, qint32 audio_port);

    void sendInitializationResponse(QString service_name, QString mac_address);

    void finished();
};
