#pragma once

#include <QMap>
#include <QObject>
#include <ReciverAudio.h>
#include <ReciverVideo.h>
#include <TransmiterAudio.h>
#include <TransmiterVideo.h>
#include <cstring>

class SessionManager : public QObject
{
private:
    Q_OBJECT
    QMap<QPair<QString, QString>, std::shared_ptr<Session>> live_sessions; // map<session_id, session>
	// void handleException(GstreamerError error);	 // Provides flowless application work after gstreamer errors.
public:
	SessionManager();

public slots:
    void onStartVideoSession(const QString &local_ip, const QString &ip6);

    void onStartAudioSession(const QString &local_ip, const QString &ip6);

    void onKillVideoSession(const QString &local_ip);

	void onKillAudioSession(const QString &local_ip);

	void onStartVideoReciver(const QString &local_ip);

	void onStartAudioReciver(const QString &local_ip);

	void onKillVideoReciver(const QString &local_ip);

	void onKillAudioReciver(const QString &local_ip);

signals:
    // All used to notify MainWindow about SessionManager events.
    void sendErrorOccured(const QString &error_string);

    void sendVideoSessionStarted();

    void sendAudioSessionStarted();

    void sendVideoSessionKilled();

    void sendAudioSessionKilled();

    void sendStartReciver(const QString &local_ip6, const QString &session_type);

    //void sendStartAudioReciver(const QString &local_ip6);
};
