#pragma once

#include <gst/gst.h>
#include <string.h>

#include <QString>
#include <QThread>

#include "Session.h"

class ReciverAudio : public Session {
private:
	Q_OBJECT

	void addLinkVideo();

	void addLinkAudio();

	void startReceive();

	static gboolean busCallback(GstBus* bus, GstMessage* msg, gpointer data);

public:
	explicit ReciverAudio(const qint16 audio_port);

	~ReciverAudio();

public slots:
	void onStartSession() override;

	void onKillSession() override;
};
