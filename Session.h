#ifndef SESSION_H
#define SESSION_H

#include <QThread>
#include <QString>


extern "C" {
#include <glib-2.0/glib.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
}

typedef struct _CustomData {
    gboolean is_live;
    GstElement* pipeline;
    GMainLoop* loop;
    GstBus *bus;
    GstMessage *msg;
} CustomData;

class Session : public QThread
{
public:
    // overriding the QThread's run() method
    void run() = 0;

protected:
    QString ip;//fix_it local_ip
    QString port;
    CustomData data;

};

#endif // SESSION_H

