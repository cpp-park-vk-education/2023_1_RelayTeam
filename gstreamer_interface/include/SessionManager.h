#pragma once

#include <QMap>
#include <QObject>
#include "QtNetwork/qhostaddress.h"
#include <ReciverAudio.h>
#include <ReciverVideo.h>
#include <TransmiterAudio.h>
#include <TransmiterVideo.h>
#include <cstring>

class SessionManager : public QObject
{
private:
    Q_OBJECT
    QHash<QPair<QHostAddress, QString>, std::shared_ptr<Session>>
        live_sessions; // map<session_id, session>
    // void handleException(GstreamerError error);	 // Provides flowless application work after gstreamer errors.
public:
	SessionManager();

public slots:
    void onStartVideoSession(const QHostAddress &local_ip6);

    void onStartAudioSession(const QHostAddress &local_ip6);

    void onKillVideoSession(const QHostAddress &local_ip6);

    void onKillAudioSession(const QHostAddress &local_ip6);

    void onStartReceivingSession(const qint16 &video_port,
                                 const qint16 &audio_port,
                                 const QString &session_type);

    void onKillVideoReciver(const QHostAddress &local_ip6);

    void onKillAudioReciver(const QHostAddress &local_ip6);

    void onSetVideoPortsTransmiter(const QHostAddress &local_ip6,
                                   const qint16 &video_port,
                                   const qint16 &audio_port);

    void onSetAudioPortsTransmiter(const QHostAddress &local_ip6,
                                   const qint16 &video_port,
                                   const qint16 &audio_port);

signals:
    // All used to notify MainWindow about SessionManager events.
    void sendErrorOccured(const QString &error_string);

    void sendVideoSessionStarted();

    void sendAudioSessionStarted();

    void sendVideoSessionKilled();

    void sendAudioSessionKilled();

    void sendStartReciver(const QHostAddress &local_ip6, const QString &session_type);

    //void sendStartAudioReciver(const QString &local_ip6);
};
