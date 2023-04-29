#ifndef RECIVERAUDIO_H
#define RECIVERAUDIO_H

#include <QString>
#include <QThread>
#include <Session.h>
#include <gst/gst.h>
#include <string.h>

class ReciverAudio : public Session
{
public:
    explicit ReciverAudio(QString port_to_reciving);
    ~ReciverAudio();

    void run();
    int start_reciver();

public slots:
    void onStartAudioSession();

    void onKillAudioSession();

signals:
    void sendAudioSessionStarted();

    void sendAudioSessionKilled();

private:
    void addLinkVideo();

    void addLinkAudio();

    void startReceive();

    gboolean bus_callback(GstBus *bus, GstMessage *msg, gpointer data);

    typedef struct _CustomData
    {
        gboolean is_live;
        GstElement *pipeline = NULL;
        GMainLoop *loop;
        GstBus *bus;
        GstMessage *msg;
    } CustomData;

    CustomData data;
};

#endif // RECIVERAUDIO_H
