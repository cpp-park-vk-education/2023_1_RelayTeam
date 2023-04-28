#ifndef TRANSMITTER_H
#define TRANSMITER_H

#include <QThread>
#include <QString>
#include <Session.h>
#include <gst/gst.h>

class Transmiter : public Session
{
private:
    gboolean on_bus_message(GstBus *bus, GstMessage *message, gpointer user_data);

    void addLinkVideo();

    void addLinkAudio();

    void startSend();

public:
    // constructor
    // set name using initializer
    explicit Transmiter(const QString &local_ip);
    ~Transmiter();

    // overriding the QThread's run() method
    void run();

    int start_transmit();

public slots:
    void onStartAudioSession();

    void onStartVideoSession();

    void onKillVideoSession();

    void onKillAudioSession();

signals:
    void sendVideoSessionStarted();

    void sendAudioSessionStarted();

    void sendVideoSessionKilled();

    void sendAudioSessionKilled();

    typedef struct _CustomData
    {
        gboolean is_live;
        GstElement *pipeline = NULL;
        GMainLoop* loop;
        GstBus *bus;
        GstMessage *msg;
    } CustomData;

    CustomData data;
    QString ip;
    QString port;
};


#endif // TRANSMITER_H
