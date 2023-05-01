#pragma once

#include <gst/gst.h>

#include <QDebug>
#include <QString>
#include <QThread>
#include <QtNetwork/QHostAddress>

#include "Session.h"

class TransmiterVideo : public Session {
private:
	Q_OBJECT

	void addLinkVideo();

    void addLinkAudio();

    void startSend();

	static gboolean onBusMessage(GstBus* bus, GstMessage* message, gpointer user_data);

public:
	explicit TransmiterVideo(const QHostAddress& local_ip6_, const qint16 video_port_, const qint16 audio_port_);

	~TransmiterVideo();

public slots:
	void onStartSession() override;

	void onKillSession() override;

signals:
    void sendVideoSessionStarted();

    void sendVideoSessionKilled();
};
