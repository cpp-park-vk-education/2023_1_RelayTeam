#include "SessionManager.h"
#include <cstring>

SessionManager::SessionManager() {}

void SessionManager::startVideoSession(const QString &local_ip)
{
    Session *session = new TransmiterVideo(local_ip);

    QPair<QString, QString> key = qMakePair(QString("127.0.0.1"), QString("TransmiterVideo"));
    std::shared_ptr<Session> myTransmitter(std::move(session));
    live_sessions.insert(key, std::move(myTransmitter));

    session->run();
}

void SessionManager::startAudioSession(const QString &local_ip)
{
    Session *session = new TransmiterAudio(local_ip);

    QPair<QString, QString> key = qMakePair(QString("127.0.0.1"), QString("TransmiterAudio"));
    std::shared_ptr<Session> myTransmitter(std::move(session));
    live_sessions.insert(key, std::move(myTransmitter));

    session->run();
}

void SessionManager::killVideoSession(const QString &local_ip)
{
    QString key = "TransmiterVideo";
    auto it = live_sessions.find(qMakePair(local_ip, key));
    if (it != live_sessions.end()) {
        auto shared_ptr = *it;
        auto value_ptr = it.value();
        shared_ptr.get()->~Session();
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
        auto shared_ptr = *it;
        auto value_ptr = it.value();

        shared_ptr.get()->~Session();
        live_sessions.erase(it);

    } else {
        // элемент не найден
    }
}
