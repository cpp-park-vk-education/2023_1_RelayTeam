#include "SessionManager.h"

SessionManager::SessionManager() {}

void SessionManager::onStartVideoSession(const QHostAddress local_ip6) {
	QPair<QHostAddress, QString> key = qMakePair(QHostAddress(local_ip6), QString("TransmiterVideo"));
	emit sendStartReciver(local_ip6, "video");
}

void SessionManager::onStartAudioSession(const QHostAddress local_ip6) {
	QPair<QHostAddress, QString> key = qMakePair(QHostAddress(local_ip6), QString("TransmiterAudio"));
	emit sendStartReciver(local_ip6, "audio");
}

void SessionManager::onKillVideoSession(const QHostAddress local_ip6) {
	QString key = "TransmiterVideo";
	if (live_sessions.contains(qMakePair(local_ip6, key))) {
		auto it = live_sessions[qMakePair(local_ip6, key)];
		QMetaObject::invokeMethod(it.get(), "onKillVideoSession", Qt::QueuedConnection);
		live_sessions.remove(qMakePair(local_ip6, key));
	}
}

void SessionManager::onKillAudioSession(const QHostAddress local_ip6) {
	QString key = "TransmiterAudio";
	if (live_sessions.contains(qMakePair(local_ip6, key))) {
		auto it = live_sessions[qMakePair(local_ip6, key)];
		// it->onStartAudioSession();
		QMetaObject::invokeMethod(it.get(), "onKillAudioSession", Qt::QueuedConnection);
		live_sessions.remove(qMakePair(local_ip6, key));
	}
}

void SessionManager::onStartReceivingSession(const QMdnsEngine::Message message_received, const QString session_type) {
	const qint16& video_port = 5228;
	const qint16& audio_port = 5229;

	if (session_type == "audio") {
		QPair<QHostAddress, QString> key = qMakePair(QHostAddress("local_ip"), QString("ReciverVideo"));
		auto it = std::make_unique<ReciverAudio>(audio_port);

		QThread* thread = new QThread();
		it.get()->moveToThread(thread);
		thread->start();

		QMetaObject::invokeMethod(static_cast<ReciverAudio*>(it.get()), "onStartAudioSession", Qt::QueuedConnection);
		live_sessions.insert(key, std::move(it));
		emit sendSetPorts(message_received, -1, audio_port);  // use -1 for unused ports
	}

	if (session_type == "video") {
		QPair<QHostAddress, QString> key = qMakePair(QHostAddress("local_ip"), QString("ReciverAudio"));
		auto it = std::make_unique<ReciverVideo>(video_port, audio_port);

		QThread* thread = new QThread();
		it.get()->moveToThread(thread);
		thread->start();

		QMetaObject::invokeMethod(static_cast<ReciverVideo*>(it.get()), "onStartVideoSession", Qt::QueuedConnection);
		live_sessions.insert(key, std::move(it));
		emit sendSetPorts(message_received, video_port, audio_port);  // use -1 for unused ports
	}
}

void SessionManager::onKillVideoReciver(const QHostAddress local_ip6) {
	QString key = "ReciverVideo";
	if (live_sessions.contains(qMakePair(local_ip6, key))) {
		auto it = live_sessions[qMakePair(local_ip6, key)];
		// it->quit();
		live_sessions.remove(qMakePair(local_ip6, key));
	}
}

void SessionManager::onKillAudioReciver(const QHostAddress local_ip6) {
	QString key = "ReciverAudio";
	if (live_sessions.contains(qMakePair(local_ip6, key))) {
		auto it = live_sessions[qMakePair(local_ip6, key)];
		// it->quit();
		live_sessions.remove(qMakePair(local_ip6, key));
	}
}

void SessionManager::onReceivedPorts(const QHostAddress local_ip6, qint16 video_port, qint16 audio_port) {
	if (video_port > -1 && audio_port > -1) {
		QPair<QHostAddress, QString> key = qMakePair(QHostAddress(local_ip6), QString("TransmiterVideo"));
		auto it = std::make_unique<TransmiterVideo>(local_ip6, video_port, audio_port);

		QThread* thread = new QThread();
		it.get()->moveToThread(thread);
		thread->start();

		QMetaObject::invokeMethod(it.get(), "onStartVideoSession", Qt::QueuedConnection);
		live_sessions.insert(key, std::move(it));
		return;
	}
	if (video_port > -1) {
		// video only pipeline
		return;
	}
	if (audio_port > -1) {
		QPair<QHostAddress, QString> key = qMakePair(QHostAddress(local_ip6), QString("TransmiterAudio"));
		auto it = std::make_unique<TransmiterAudio>(local_ip6, 5000);

		QThread* thread = new QThread();
		it.get()->moveToThread(thread);
		thread->start();

		QMetaObject::invokeMethod(it.get(), "onStartAudioSession", Qt::QueuedConnection);
		live_sessions.insert(key, std::move(it));
		return;
	}
}
