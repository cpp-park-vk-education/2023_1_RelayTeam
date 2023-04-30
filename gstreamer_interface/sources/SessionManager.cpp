#include "SessionManager.h"

SessionManager::SessionManager() {}

void SessionManager::onStartVideoSession(const QHostAddress &local_ip6)
{
    QPair<QHostAddress, QString> key = qMakePair(QHostAddress(local_ip6),
                                                 QString("TransmiterVideo"));
    auto it = std::make_unique<TransmiterVideo>(local_ip6, 5001, 5000);
    it->onStartVideoSession();
    emit sendStartReciver(local_ip6, "video");
    live_sessions.insert(key, std::move(it));
}

void SessionManager::onStartAudioSession(const QHostAddress &local_ip6)
{
    QPair<QHostAddress, QString> key = qMakePair(QHostAddress(local_ip6),
                                                 QString("TransmiterAudio"));
    auto it = std::make_unique<TransmiterAudio>(local_ip6, 5000);
    it->onStartAudioSession();
    emit sendStartReciver(local_ip6, "audio");
    live_sessions.insert(key, std::move(it));
}

void SessionManager::onKillVideoSession(const QHostAddress &local_ip6)
{
    QString key = "TransmiterVideo";
    if (live_sessions.contains(qMakePair(local_ip6, key))) {
        auto it = live_sessions[qMakePair(local_ip6, key)];
        QMetaObject::invokeMethod(it.get(), "onKillVideoSession", Qt::QueuedConnection);
        live_sessions.remove(qMakePair(local_ip6, key));
    }
}

void SessionManager::onKillAudioSession(const QHostAddress &local_ip6)
{
    QString key = "TransmiterAudio";
    if (live_sessions.contains(qMakePair(local_ip6, key))) {
        auto it = live_sessions[qMakePair(local_ip6, key)];
        //it->onStartAudioSession();
        QMetaObject::invokeMethod(it.get(), "onKillAudioSession", Qt::QueuedConnection);
        live_sessions.remove(qMakePair(local_ip6, key));
    }
}

void SessionManager::onStartReceivingSession(const qint16 &video_port,
                                             const qint16 &audio_port,
                                             const QString &session_type)
{
    if (session_type == "audio") {
        QPair<QHostAddress, QString> key = qMakePair(QHostAddress("local_ip"),
                                                     QString("ReciverVideo"));
        auto it = std::make_unique<ReciverAudio>(audio_port);
        it->onStartAudioSession();
        live_sessions.insert(key, std::move(it));
    }

    if (session_type == "video") {
        QPair<QHostAddress, QString> key = qMakePair(QHostAddress("local_ip"),
                                                     QString("ReciverAudio"));
        auto it = std::make_unique<ReciverVideo>(video_port, audio_port);
        it->onStartVideoSession();
        live_sessions.insert(key, std::move(it));
    }
}

void SessionManager::onKillVideoReciver(const QHostAddress &local_ip6)
{
    QString key = "ReciverVideo";
    if (live_sessions.contains(qMakePair(local_ip6, key))) {
        auto it = live_sessions[qMakePair(local_ip6, key)];
        //it->quit();
        live_sessions.remove(qMakePair(local_ip6, key));
    }
}

void SessionManager::onKillAudioReciver(const QHostAddress &local_ip6)
{
    QString key = "ReciverAudio";
    if (live_sessions.contains(qMakePair(local_ip6, key))) {
        auto it = live_sessions[qMakePair(local_ip6, key)];
        //it->quit();
        live_sessions.remove(qMakePair(local_ip6, key));
    }
}

void SessionManager::onSetVideoPortsTransmiter(const QHostAddress &local_ip6,
                                               const qint16 &video_port,
                                               const qint16 &audio_port)
{
    QPair<QHostAddress, QString> key = qMakePair(QHostAddress(local_ip6),
                                                 QString("TransmiterVideo"));
    auto it = std::make_unique<TransmiterVideo>(local_ip6, video_port, audio_port);

    it->onStartVideoSession();
    live_sessions.insert(key, std::move(it));
}

void SessionManager::onSetAudioPortsTransmiter(const QHostAddress &local_ip6,
                                               const qint16 &video_port,
                                               const qint16 &audio_port)
{
    QPair<QHostAddress, QString> key = qMakePair(QHostAddress(local_ip6),
                                                 QString("TransmiterAudio"));
    auto it = std::make_unique<TransmiterAudio>(local_ip6, audio_port);

    it->onStartAudioSession();
    live_sessions.insert(key, std::move(it));
}
