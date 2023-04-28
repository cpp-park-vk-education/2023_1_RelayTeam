#include "SessionManager.h"
#include <cstring>

void SessionManager::startVideoSession(const QString &local_ip)
{
    //Transmiter trans(local_ip);
    Session *session = new Transmiter(local_ip);

    session->run();
}

void SessionManager::startAudioSession(const QString &local_ip)
{
    Session *session = new Transmiter(local_ip);

    session->run();
}

void SessionManager::killVideoSession(const QString &local_ip)
{

}

void SessionManager::killAudioSession(const QString &local_ip)
{

}
