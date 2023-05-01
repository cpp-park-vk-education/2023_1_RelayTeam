#pragma once

#include <gst/gst.h>
#include <string.h>

#include <QString>
#include <QThread>

#include "Session.h"

class ReciverVideo : public Session {
	Q_OBJECT
public:
	explicit ReciverVideo(const qint16 video_port, const qint16 audio_port);

	~ReciverVideo();

public slots:
	void onStartSession() override;

	void onKillSession() override;

signals:
    void sendVideoSessionStarted();

    void sendVideoSessionKilled();

private:
    void addLinkVideo();

    void addLinkAudio();

    void startReceive();

	static gboolean busCallback(GstBus* bus, GstMessage* msg, gpointer data);
};
