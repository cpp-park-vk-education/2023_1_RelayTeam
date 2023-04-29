#pragma once

#include <QThread>
#include <QString>
#include <gst/gst.h>


class Session : public QThread
{
public:
    void run() = 0;

protected:
    typedef struct _CustomData
    {
        gboolean is_live;
        GstElement *pipeline = NULL;
        GMainLoop *loop;
        GstBus *bus;
        GstMessage *msg;
    } CustomData;

    CustomData data;
    QString local_ip4;
    QString local_ip6;
    qint16 port_to_transmitter;
};
