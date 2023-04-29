#pragma once

#include <QString>
#include <QThread>
#include <Session.h>
#include <gst/gst.h>

class TransmiterAudio : public Session
{
private:
    gboolean onBusMessage(GstBus *bus, GstMessage *message, gpointer user_data);

    void addLinkVideo();

    void addLinkAudio();

    void startSend();

public:
    // constructor
    // set name using initializer
    explicit TransmiterAudio(const QString &local_ip4, const QString &ip6);
    ~TransmiterAudio();

    // overriding the QThread's run() method
    void run();

    int startTransmit();

public slots:
    void onStartAudioSession();

    void onKillAudioSession();

signals:
    void sendAudioSessionStarted();

    void sendAudioSessionKilled();
};
