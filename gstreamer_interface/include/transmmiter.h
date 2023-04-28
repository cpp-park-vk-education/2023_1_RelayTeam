#ifndef TRANSMITTER_H
#define TRANSMITER_H

#include <QThread>
#include <QString>
#include <Session.h>
#include <gst/gst.h>

class Transmiter : public Session
{
public:
    // constructor
    // set name using initializer
    explicit Transmiter(QString port_to_transmit);

    // overriding the QThread's run() method
    void run();
    int start_transmit();

private:
    gboolean on_bus_message (GstBus *bus, GstMessage *message, gpointer user_data);
    void addLinkVideo();
    void addLinkAudio();
    void startAudioSession();
    void startVideoSession();
    void startSend();
    void killVideoSession();
    void killAudioSession();


    typedef struct _CustomData {
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
