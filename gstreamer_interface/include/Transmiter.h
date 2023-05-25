#pragma once

#include <gst/gst.h>

#include <QDebug>
#include <QString>
#include <QThread>
#include <QtNetwork/QHostAddress>

#include "Session.h"

class Transmiter : public Session {
private:
    Q_OBJECT

    void addLinkVideo();

    void addLinkAudio();

    void removeVideo();

    void removeAudio();

    void startSend();

public:
    explicit Transmiter(const QHostAddress& ip_address_, const qint32 video_port_, const qint32 audio_port_);
    explicit Transmiter(const QHostAddress& ip_address_, const qint32 audio_port_);

    ~Transmiter();

public slots:
    void onEnableVideo() override;

    void onEnableAudio() override;

    void onDisableVideo() override;

    void onDisableAudio() override;

    void onStartSession() override;

    void onKillSession() override;

    void onSetVolume(float volume) override;

    void onSetBitrate(const int bitrate) override;
};
