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

    public:
        SessionManager();

	~SessionManager();

public slots:
	void onStartVideoSession(const QHostAddress ip_address);

	void onStartAudioSession(const QHostAddress ip_address);

	void onKillVideoSession(const QHostAddress ip_address);

	void onKillAudioSession(const QHostAddress ip_address);

	void onStartReceivingSession(const QHostAddress ip_address, const QString session_type);

	void onKillVideoReciver(const QHostAddress ip_address);

        void onKillAudioReciver(const QHostAddress ip_address);

        void onReceivedPorts(const QHostAddress ip_address, qint32 video_port, qint32 audio_port);

    signals:
        void sendErrorOccured(const QString error_string);

        void sendVideoSessionStarted();

	void sendAudioSessionStarted();

	void sendVideoSessionKilled();

	void sendAudioSessionKilled();

	void sendKillAll();

	void sendStartReciver(const QHostAddress local_ip6, const QString session_type);

	void sendSetPorts(const QHostAddress ip_address, qint32 video_port, qint32 audio_port);
};
