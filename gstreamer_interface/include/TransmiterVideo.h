#pragma once

#include <QDebug>
#include <QString>
#include <QThread>
#include <Session.h>
#include <gst/gst.h>

class TransmiterVideo : public Session
{
private:
    void addLinkVideo();

    void addLinkAudio();

    void startSend();

public:
    explicit TransmiterVideo(const QString &local_ip4, const QString &ip6);
    ~TransmiterVideo();

    void run();

    int startTransmit();

public slots:
    void onStartVideoSession();

    void onKillVideoSession();

signals:
    void sendVideoSessionStarted();

    void sendVideoSessionKilled();
};
