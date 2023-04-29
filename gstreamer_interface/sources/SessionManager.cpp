#include "SessionManager.h"
#include <cstring>

SessionManager::SessionManager() {}

void SessionManager::onStartVideoSession(const QString &local_ip, const QString &ip6)
{
    QPair<QString, QString> key = qMakePair(QString(local_ip), QString("TransmiterVideo"));
    auto it = std::make_unique<TransmiterVideo>(local_ip, ip6);
    it->start();
    live_sessions.insert(key, std::move(it));
}

void SessionManager::onStartAudioSession(const QString &local_ip)
{
    QPair<QString, QString> key = qMakePair(QString(local_ip), QString("TransmiterAudio"));
    auto it = std::make_unique<TransmiterAudio>(local_ip);
    it->start();
    live_sessions.insert(key, std::move(it));
}

void SessionManager::onKillVideoSession(const QString &local_ip)
{
    QString key = "TransmiterVideo";
    if (live_sessions.contains(qMakePair(local_ip, key))) {
        auto it = live_sessions[qMakePair(local_ip, key)];
        it->quit();
        live_sessions.remove(qMakePair(local_ip, key));
    }
}

void SessionManager::onKillAudioSession(const QString &local_ip)
{
    QString key = "TransmiterAudio";
    if (live_sessions.contains(qMakePair(local_ip, key))) {
        auto it = live_sessions[qMakePair(local_ip, key)];
        it->quit();
        live_sessions.remove(qMakePair(local_ip, key));
    }
}

void SessionManager::onStartVideoReciver(const QString &local_ip)
{
    QPair<QString, QString> key = qMakePair(QString(local_ip), QString("ReciverVideo"));
    auto it = std::make_unique<ReciverVideo>(local_ip);
    it->start();
    live_sessions.insert(key, std::move(it));

    //emit sendStartReciver(local_ip6, "");
}

void SessionManager::onStartAudioReciver(const QString &local_ip)
{
    QPair<QString, QString> key = qMakePair(QString(local_ip), QString("ReciverAudio"));
    auto it = std::make_unique<ReciverVideo>(local_ip);
    it->start();
    live_sessions.insert(key, std::move(it));
}

void SessionManager::onKillVideoReciver(const QString &local_ip)
{
    QString key = "ReciverVideo";
    if (live_sessions.contains(qMakePair(local_ip, key))) {
        auto it = live_sessions[qMakePair(local_ip, key)];
        it->quit();
        live_sessions.remove(qMakePair(local_ip, key));
    }
}

void SessionManager::onKillAudioReciver(const QString &local_ip)
{
    QString key = "ReciverAudio";
    if (live_sessions.contains(qMakePair(local_ip, key))) {
        auto it = live_sessions[qMakePair(local_ip, key)];
        it->quit();
        live_sessions.remove(qMakePair(local_ip, key));
    }
}
