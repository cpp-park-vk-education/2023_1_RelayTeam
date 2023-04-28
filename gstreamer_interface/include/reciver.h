#ifndef RECIVER_H
#define RECIVER_H

#include <QThread>
#include <Session.h>
#include <QString>
#include <gst/gst.h>
#include <string.h>

class Reciver : public Session
{
    public:
        // constructor
        // set name using initializer
        explicit Reciver(QString port_to_reciving);
        ~Reciver();

        // overriding the QThread's run() method
        void run();
        int start_reciver();

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

    private:
        void addLinkVideo();

        void addLinkAudio();

        void startReceive();

        gboolean bus_callback(GstBus *bus, GstMessage *msg, gpointer data);

        typedef struct _CustomData {
            gboolean is_live;
            GstElement *pipeline = NULL;
            GMainLoop* loop;
            GstBus *bus;
            GstMessage *msg;
        } CustomData;

        CustomData data;


};


#endif // RECIVER_H
