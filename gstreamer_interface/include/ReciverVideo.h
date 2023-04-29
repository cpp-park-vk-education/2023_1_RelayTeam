#ifndef RECIVERVIDEO_H
#define RECIVERVIDEO_H

#include <QString>
#include <QThread>
#include <Session.h>
#include <gst/gst.h>
#include <string.h>

class ReciverVideo : public Session
{
public:
    explicit ReciverVideo(QString port_to_reciving);
    //~ReciverVideo();

    void run();
    int start_reciver();

public slots:
    void onStartVideoSession();

    void onKillVideoSession();

signals:
    void sendVideoSessionStarted();

    void sendVideoSessionKilled();

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

#endif // RECIVERVIDEO_H
