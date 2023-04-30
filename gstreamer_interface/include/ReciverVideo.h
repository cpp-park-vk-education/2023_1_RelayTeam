#pragma once

#include <QString>
#include <QThread>
#include <Session.h>
#include <gst/gst.h>
#include <string.h>

class ReciverVideo : public Session
{
public:
    explicit ReciverVideo(const qint16 &video_port, const qint16 &audio_port);

    void startReciver();

public slots:
    void onStartVideoSession();

    void onKillVideoSession();

signals:
    void sendVideoSessionStarted();

    void sendVideoSessionKilled();

private:
    void addLinkVideo();

    void addLinkAudio();

    void startReceive();

    static gboolean bus_callback(GstBus *bus, GstMessage *msg, gpointer data);
};
