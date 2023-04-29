#pragma once

#include <QString>
#include <QThread>
#include <Session.h>
#include <gst/gst.h>

class TransmiterAudio : public Session
{
private:
    gboolean on_bus_message(GstBus *bus, GstMessage *message, gpointer user_data);

    void addLinkVideo();

    void addLinkAudio();

    void startSend();

public:
    // constructor
    // set name using initializer
    explicit TransmiterAudio(const QString &local_ip);
    ~TransmiterAudio();

    // overriding the QThread's run() method
    void run();

    int start_transmit();

public slots:
    void onStartAudioSession();

    void onKillAudioSession();

signals:
    void sendAudioSessionStarted();

    void sendAudioSessionKilled();

    typedef struct _CustomData
    {
        gboolean is_live;
        GstElement *pipeline = NULL;
        GMainLoop *loop;
        GstBus *bus;
        GstMessage *msg;
    } CustomData;

    CustomData data;
    QString ip;
    QString port;
};
