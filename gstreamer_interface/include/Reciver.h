#pragma once

#include <gst/gst.h>
#include <string.h>

#include <QString>
#include <QThread>
#include <QTimer>
#include <QWidget>

#include "Session.h"

class Reciver : public Session {
private:
    Q_OBJECT

    QWidget* window;

    void addLinkVideo();

    void addLinkAudio();

    void removeVideo();

    void removeAudio();

    void startReceive();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

public:
    explicit Reciver(const QHostAddress& ip_address_, const qint16 video_port, const qint16 audio_port);
    explicit Reciver(const QHostAddress& ip_address_, const qint16 audio_port);

    ~Reciver();

public slots:
    void onEbableCamera() override;

    void onEnableVideo() override;

    void onEnableAudio() override;

    void onStartSession() override;

    void onKillSession() override;

    void onSetVolume(const float volume);

signals:
    void mainWindowClosed();
};
