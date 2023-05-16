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

public:
	explicit TransmiterAudio(const QHostAddress& ip_address_, const qint32 audio_port_);

	~TransmiterAudio();

public slots:
	void onStartSession() override;

	void onKillSession() override;
};
