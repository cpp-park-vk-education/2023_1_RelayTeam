#include "SessionManager.h"
#include <cstring>

SessionManager::SessionManager() {}

void SessionManager::startVideoSession(const QString &local_ip)
{
    QPair<QString, QString> key = qMakePair(QString("127.0.0.1"), QString("TransmiterVideo"));
    live_sessions.insert(key, std::make_unique<TransmiterVideo>(local_ip));
}

void SessionManager::startAudioSession(const QString &local_ip)
{
    Session *session = new TransmiterAudio(local_ip);

    QPair<QString, QString> key = qMakePair(QString("127.0.0.1"), QString("TransmiterAudio"));
    live_sessions.insert(key, std::make_unique<TransmiterVideo>(local_ip));
}

void SessionManager::killVideoSession(const QString &local_ip)
{
    QString key = "TransmiterVideo";
    auto it = live_sessions.find(qMakePair(local_ip, key));
    if (it != live_sessions.end()) {
        it.value()->quit();
        live_sessions.erase(it);

    } else {
        // элемент не найден
    }
}

void SessionManager::killAudioSession(const QString &local_ip)
{
    QString key = "TransmiterAudio";
    auto it = live_sessions.find(qMakePair(local_ip, key));
    if (it != live_sessions.end()) {
        it.value()->quit();
        live_sessions.erase(it);

    } else {
        // элемент не найден
    }
}
