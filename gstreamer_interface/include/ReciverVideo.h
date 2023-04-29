#pragma once

#include <QString>
#include <QThread>
#include <Session.h>
#include <gst/gst.h>
#include <string.h>

class ReciverVideo : public Session
{
public:
    explicit ReciverVideo(QString port_to_reciving);
    //~ReciverVideo();

    void run();
    int startReciver();

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
};
