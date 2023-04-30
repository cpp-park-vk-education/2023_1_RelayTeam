#pragma once

#include <QString>
#include <QThread>
#include <Session.h>
#include <gst/gst.h>
#include <string.h>

class ReciverAudio : public Session
{
public:
    explicit ReciverAudio(const qint16 &audio_port);
    ~ReciverAudio();

    void startReciver();

public slots:
    void onStartAudioSession();

    void onKillAudioSession();

signals:
    void sendAudioSessionStarted();

    void sendAudioSessionKilled();

private:
    void addLinkVideo();

    void addLinkAudio();

    void startReceive();

    static gboolean busCallback(GstBus *bus, GstMessage *msg, gpointer data);
};
