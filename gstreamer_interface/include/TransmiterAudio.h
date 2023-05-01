#pragma once

#include <gst/gst.h>
#include <QString>
#include <QThread>
#include <QtNetwork/QHostAddress>
#include <Session.h>

class TransmiterAudio : public Session {
private:
	Q_OBJECT

	static gboolean onBusMessage(GstBus *bus, GstMessage *message, gpointer user_data);

	void addLinkAudio();

    void startSend();

public:
	explicit TransmiterAudio(const QHostAddress &local_ip6_, const qint16 audio_port_);

	~TransmiterAudio();

    void startTransmit();

public slots:
    void onStartAudioSession();

    void onKillAudioSession();

signals:
    void sendAudioSessionStarted();

    void sendAudioSessionKilled();
};
