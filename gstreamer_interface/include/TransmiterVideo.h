#pragma once

#include <QDebug>
#include <QString>
#include <QThread>
#include "QtNetwork/qhostaddress.h"
#include <Session.h>
#include <gst/gst.h>

class TransmiterVideo : public Session
{
private:
    void addLinkVideo();

    void addLinkAudio();

    void startSend();

    static gboolean bus_message(GstBus *bus, GstMessage *message, gpointer user_data);

public:
    explicit TransmiterVideo(const QHostAddress &local_ip6_,
                             const qint16 &video_port_,
                             const qint16 &audio_port_);
    ~TransmiterVideo();

    //void run();
    void startTransmit();

public slots:
    void onStartVideoSession();

    void onKillVideoSession();

signals:
    void sendVideoSessionStarted();

    void sendVideoSessionKilled();
};
