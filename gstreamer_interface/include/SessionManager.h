#pragma once

#include <ReciverAudio.h>
#include <ReciverVideo.h>
#include <TransmiterAudio.h>
#include <TransmiterVideo.h>
#include <qmdnsengine/message.h>

#include <QMap>
#include <QObject>
#include <QtNetwork/QHostAddress>
#include <cstring>

class SessionManager : public QObject {
private:
    Q_OBJECT
	QHash<QPair<QHostAddress, QString>, std::shared_ptr<Session>> live_sessions;  // map<session_id, session>
    // void handleException(GstreamerError error);	 // Provides flowless application work after gstreamer errors.

	void startThread(Session* session);

public:
	SessionManager();

	~SessionManager();

public slots:
	void onStartVideoSession(const QHostAddress local_ip6);

	void onStartAudioSession(const QHostAddress local_ip6);

	void onKillVideoSession(const QHostAddress local_ip6);

	void onKillAudioSession(const QHostAddress local_ip6);

	void onStartReceivingSession(const QMdnsEngine::Message message_received, const QString session_type);

	void onKillVideoReciver(const QHostAddress local_ip6);

	void onKillAudioReciver(const QHostAddress local_ip6);

	void onReceivedPorts(const QHostAddress local_ip6, qint16 video_port, qint16 audio_port);

signals:
	void sendErrorOccured(const QString error_string);

	void sendVideoSessionStarted();

	void sendAudioSessionStarted();

	void sendVideoSessionKilled();

	void sendAudioSessionKilled();

	void sendKillAll();

	void sendStartReciver(const QHostAddress local_ip6, const QString session_type);

	void sendSetPorts(const QMdnsEngine::Message message_received, qint16 video_port, qint16 audio_port);
};
