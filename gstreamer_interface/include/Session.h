#pragma once

#include <gst/gst.h>
#include <QString>
#include <QThread>
#include <QtNetwork/QHostAddress>

class Session : public QObject {
	Q_OBJECT
protected:
	typedef struct _CustomData {
        gboolean is_live;
        GstElement *pipeline = NULL;
        GMainLoop *loop;
        GstBus *bus;
        GstMessage *msg;
    } CustomData;

	Session(const QHostAddress local_ip6_, const qint16 video_port_, const qint16 audio_port_);
	Session(const QHostAddress local_ip6_, const qint16 audio_port_);
	Session(const qint16 video_port_, const qint16 audio_port_);
	Session(const qint16 audio_port_);

	CustomData data;
	QHostAddress local_ip6;
	qint16 video_port;
	qint16 audio_port;
};
