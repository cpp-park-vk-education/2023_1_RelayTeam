#pragma once

#include <gst/gst.h>
#include <string.h>

#include <QString>
#include <QThread>

#include "Session.h"

class ReciverVideo : public Session {
private:
	Q_OBJECT

	void addLinkVideo();

	void addLinkAudio();

        void startReceive();

    public:
        explicit ReciverVideo(const qint16 video_port, const qint16 audio_port);

	~ReciverVideo();

public slots:
	void onStartSession() override;

	void onKillSession() override;
};
