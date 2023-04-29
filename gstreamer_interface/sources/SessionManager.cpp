#include "SessionManager.h"

SessionManager::SessionManager() {}

void SessionManager::onStartVideoSession(const QString &local_ip, const QString &ip6) {
	QPair<QString, QString> key = qMakePair(QString(local_ip), QString("TransmiterVideo"));
	auto it = std::make_unique<TransmiterVideo>(local_ip, ip6);
	it->start();
	emit sendStartReciver(ip6, "video");
	live_sessions.insert(key, std::move(it));
}

void SessionManager::onStartAudioSession(const QString &local_ip, const QString &ip6) {
	QPair<QString, QString> key = qMakePair(QString(local_ip), QString("TransmiterAudio"));
	auto it = std::make_unique<TransmiterAudio>(local_ip, ip6);
	it->start();
	emit sendStartReciver(ip6, "audio");
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

void SessionManager::onStartReceivingSession(const QString &session_type) {
	if (session_type == "audio") {
		QPair<QString, QString> key = qMakePair(QString("local_ip"), QString("ReciverVideo"));
		auto it = std::make_unique<ReciverVideo>("local_ip");
		it->start();
		live_sessions.insert(key, std::move(it));

		// emit sendStartReciver(local_ip6, "");
	}

	if (session_type == "video") {
		QPair<QString, QString> key = qMakePair(QString("local_ip"), QString("ReciverAudio"));
		auto it = std::make_unique<ReciverVideo>("local_ip");
		it->start();
		live_sessions.insert(key, std::move(it));
	}
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
