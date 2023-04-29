#ifndef TRANSMITERVIDEO_H
#define TRANSMITERVIDEO_H

#include <QString>
#include <QThread>
#include <Session.h>
#include <gst/gst.h>

class TransmiterVideo : public Session
{
private:
    gboolean on_bus_message(GstBus *bus, GstMessage *message, gpointer user_data);

    void addLinkVideo();

    void addLinkAudio();

    void startSend();

public:
    explicit TransmiterVideo(const QString &local_ip);
    //~TransmiterVideo();

    void run();

    int start_transmit();

public slots:
    void onStartVideoSession();

    void onKillVideoSession();

signals:
    void sendVideoSessionStarted();

    void sendVideoSessionKilled();

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

#endif // TRANSMITERVIDEO_H
