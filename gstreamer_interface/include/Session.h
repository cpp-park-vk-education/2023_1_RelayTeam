#ifndef SESSION_H
#define SESSION_H

#include <QThread>
#include <QString>
#include <gst/gst.h>


class Session : public QThread
{
public:
    // overriding the QThread's run() method
    void run() = 0;

    //public slots:
    //    void startAudioSession() = 0;

    //    void startVideoSession() = 0;

    //    void killVideoSession() = 0;

    //    void killAudioSession() = 0;

    gboolean bus_callback(GstBus *bus, GstMessage *msg, gpointer data);

protected:
    QString ip;//fix_it local_ip
    QString port;

};
#endif // SESSION_H
