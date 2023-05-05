#pragma once

#include <gst/gst.h>

#include <QString>
#include <QThread>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QHostAddress>
#include <string>

class Session : public QObject {
private:
	Q_OBJECT

protected:
    static gboolean busCallback(GstBus *bus, GstMessage *msg, gpointer data);

    typedef struct _CustomData
    {
        gboolean is_live;
        GstElement *pipeline = NULL;
        GMainLoop *loop;
        GstBus *bus;
        GstMessage *msg;
    } CustomData;

    Session(const QHostAddress &ip_address_, const qint32 video_port_, const qint32 audio_port_);

    CustomData data;
    QHostAddress ip_address;
    qint32 video_port;
    qint32 audio_port;

public:
    const static char *representIP(const QHostAddress &ext_ip_address);

    bool isPipelineCreated();

public slots:
    virtual void onStartSession() = 0;

    virtual void onKillSession() = 0;

signals:
    void sendSessionStarted();

    void sendSessionKilled();
};
