#pragma once

#include <Reciver.h>
#include <Transmiter.h>

#include <QMap>
#include <QObject>
#include <QtNetwork/QHostAddress>
#include <cstring>

class SessionManager : public QObject {
private:
    Q_OBJECT
    QHash<QPair<QHostAddress, QString>, std::shared_ptr<Session>> live_sessions;  // map<session_id, session>
    // void handleException(GstreamerError error);	 // Provides flowless application work after gstreamer errors.

    void startThread(Session* session, const QString session_type);

    void killSessionIp4(const QHostAddress ip_address, const QString key);

    void killSessionIp6(const QHostAddress ip_address, const QString key);

public:
    SessionManager();

    ~SessionManager();

public slots:
    void onStartVideoSession(const QHostAddress ip_address);

    void onStartCameraSession(const QHostAddress ip_address);

    void onStartAudioSession(const QHostAddress ip_address);

    void onKillVideoSession(const QHostAddress ip_address);

    void onKillAudioSession(const QHostAddress ip_address);

    void onStartReceivingSession(const QHostAddress ip_address, const QString session_type);

    void onKillCameraSession(const QHostAddress ip_address);

    void onReceivedPorts(const QHostAddress ip_address, qint32 video_port, qint32 audio_port);

    void onReceivedCameraPorts(const QHostAddress ip_address, qint32 video_port);

    void onCloseWindow(const QHostAddress& ip_address_, const QString session_type);

    void onSetVolume(const QHostAddress ip_address, const int volume);

    void onSetBitrate(const QHostAddress ip_address, const int bitrate);

    void setCameraCaptureMode(const QHostAddress ip_address);

    void setScreenCaptureMode(const QHostAddress ip_address);

signals:
    void sendErrorOccured(const QString error_string);

    void sendCameraSession(const QHostAddress ip_address, const qint32 video_port);

    void senStartCameraSession(const QHostAddress ip_address,const  qint32 video_port);

    void sendSetCameraPort(const QHostAddress ip_address, const qint32 video_port);

    void setVolume(const int volume);

    void setBitrate(const int bitrate);

    void sendVideoSessionStarted();

    void sendAudioSessionStarted();

    void sendVideoSessionKilled();

    void sendAudioSessionKilled();

    void sendKillAll();

    void setPorts(const QHostAddress ip_address, const qint32 session_type);

    void sendStartReciver(const QHostAddress local_ip6, const QString session_type);

    void sendSetPorts(const QHostAddress ip_address, const qint32 video_port, const qint32 audio_port);
};
