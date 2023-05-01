#pragma once

#include <gst/gst.h>

#include <QString>
#include <QThread>
#include <QtNetwork/QHostAddress>

#include "Session.h"

class TransmiterAudio : public Session {
private:
	Q_OBJECT

	void addLinkAudio();

    void startSend();

	static gboolean onBusMessage(GstBus* bus, GstMessage* message, gpointer user_data);

public:
	explicit TransmiterAudio(const QHostAddress& local_ip6_, const qint16 audio_port_);

	~TransmiterAudio();

public slots:
	void onStartSession() override;

	void onKillSession() override;
};
