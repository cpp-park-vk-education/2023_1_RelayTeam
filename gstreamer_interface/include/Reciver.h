#pragma once

#include <gst/gst.h>
#include <string.h>

#include <QString>
#include <QThread>

#include "Session.h"

class Reciver : public Session {
private:
    Q_OBJECT

    void addLinkVideo();

    void addLinkAudio();

    void removeVideo();

    void removeAudio();

    void startReceive();

public:
    explicit Reciver(const qint16 video_port, const qint16 audio_port);
    explicit Reciver(const qint16 audio_port);

    ~Reciver();

public slots:
    void onVolumeChanged();

    void onEnableVideo() override;

    void onEnableAudio() override;

    void onDisableVideo() override;

    void onDisableAudio() override;

    void onStartSession() override;

    void onKillSession() override;

signals:
    void mainWindowClosed();
};
