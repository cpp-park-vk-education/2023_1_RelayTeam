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

    public:
        explicit ReciverAudio(const qint16 audio_port);

	~ReciverAudio();

public slots:
	void onStartSession() override;

	void onKillSession() override;
};
