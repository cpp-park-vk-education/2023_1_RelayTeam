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

public:
	explicit TransmiterVideo(const QHostAddress& ip_address_, const qint32 video_port_, const qint32 audio_port_);

	~TransmiterVideo();

public slots:
	void onStartSession() override;

	void onKillSession() override;
};