#pragma once

#include <gst/gst.h>

#include <QString>
#include <QThread>
#include <QtNetwork/QHostAddress>

class Session : public QObject {
private:
	Q_OBJECT

protected:
	static gboolean busCallback(GstBus* bus, GstMessage* msg, gpointer data);

	typedef struct _CustomData {
		gboolean is_live;
		GstElement* pipeline = NULL;
		GMainLoop* loop;
		GstBus* bus;
		GstMessage* msg;
	} CustomData;

	Session(const QHostAddress& local_ip6_, const qint16 video_port_, const qint16 audio_port_);

	CustomData data;
	QHostAddress local_ip6;
	qint16 video_port;
	qint16 audio_port;

public slots:
	virtual void onStartSession() = 0;

	virtual void onKillSession() = 0;

signals:
	void sendSessionStarted();

	void sendSessionKilled();
};
