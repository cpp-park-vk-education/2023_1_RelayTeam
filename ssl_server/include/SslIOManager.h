#pragma once

#include <QHash>
#include <QObject>
#include <QThread>

#include "SSLServer.h"

class SslServer;

/*!
 * \brief Manages ssl server and all of it's IO operations with connections.
 */
class SslIOManager : public QObject {
private:
    Q_OBJECT

    SslServer* ssl_server;

    QHash<QHostAddress, SslConnection*> connections_map;

public:
    SslIOManager(QObject* parent = nullptr);

    ~SslIOManager();

public slots:
    /*!
     * \brief Adds connection to map of active connections.
     * \param address - Address of connection.
     */
    void onAddSslConnection(QHostAddress address);

    /*!
     * \brief Removes connection to map of active connections.
     * \param address - Address of connection.
     */
    void onRemoveSslConnection(QHostAddress address);

    /*!
     * \brief Writes text to opend socket. Creates one if doesn't exist.
     * \param address - address of receiving socket.
     * \param message - text of message.
     */
    void writeTo(QHostAddress address, QString message);

    /*!
     * \brief Sends ports from Receiver to Transmitter.
     * \param ip_address Address of transmitter.
     * \param video_port Video port of Receiver.
     * \param audio_port Audio port of Receiver.
     */
    void onSendPorts(QHostAddress ip_address, qint32 video_port, qint32 audio_port);

    /*!
     * \brief Requests initalization from a certain machine
     * \param ip_address
     */
    void onStartReciver(const QHostAddress ip_address, const QString session_type);

    /*!
     * \brief Request's  Receiver side to start receiving session.
     * \param ip_address - address of receiving side.
     * \param session_type - type of requested session.
     */
    void onRequestInitialization(const QHostAddress ip_address, QString service_name);
signals:
    /*!
     * \brief Signal that conects Connections and SessionManager.
     * \param ip_address - ip address of Transmitter.
     * \param session_type - type of Transmitter session.
     */
    void sendStartReceivingSession(QHostAddress ip_address, QString session_type);

    /*!
     * \brief Sends chosen ports from Receiver side to Transmitter side.
     * \param ipv_address - address of Transmitter.
     * \param video_port - chosen video port.
     * \param audio_port - chosen audio port.
     */
    void sendReceivedPorts(const QHostAddress ipv4_address, qint32 video_port, qint32 audio_port);

    void sendInitializationResponse(QString service_name, QString mac_address);

    void sendKillAll();
};
