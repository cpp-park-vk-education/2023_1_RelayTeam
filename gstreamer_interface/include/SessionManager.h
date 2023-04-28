#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QMap>
#include <QObject>
#include <Session.h>

class SessionManager : public QObject {
private:
    Q_OBJECT
    QMap<QString, std::unique_ptr<Session>> live_sessions;	 // map<session_id, session>

    //void handleException(GstreamerError error);	 // Provides flowless application work after gstreamer errors.

    void startSend();
    gboolean on_bus_message(GstBus *bus, GstMessage *msg);
    void addLinkVideo(const QString& local_ip);
    void addLinkAudio(const QString& local_ip);

    typedef struct _CustomData {
        gboolean is_live;
        GstElement* pipeline;
        GMainLoop* loop;
        GstBus *bus;
        GstMessage *msg;
    } CustomData;

    CustomData data;

public slots:
    void startVideoSession(const QString &local_ip);

    void startAudioSession(const QString &local_ip);

    void killVideoSession(const QString &local_ip);

    void killAudioSession(const QString &local_ip);

signals:
    // All used to notify MainWindow about SessionManager events.
    void sendErrorOccured(const QString &error_string);

    void sendVideoSessionStarted();

    void sendAudioSessionStarted();

    void sendVideoSessionKilled();

    void sendAudioSessionKilled();
};
#endif // SESSIONMANAGER_H
