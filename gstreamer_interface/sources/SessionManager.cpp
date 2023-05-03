#include "SessionManager.h"

#include <networkTools.h>

bool SessionManager::portIsBusy(QString ip, qint16 port) {
    QTcpSocket* socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);
    qint8 msecs_delay_time = 100;
    if (socket->waitForConnected(msecs_delay_time)) {
        socket->disconnectFromHost();
        return true;
    } else {
        return false;
    }
}

qint16 SessionManager::getPort() {
    qint16 startRangeSearch = 4000;
    qint16 stopRangeSearch = 5000;
    for (qint16 i = startRangeSearch; i < stopRangeSearch; i++) {
        if (!this->portIsBusy("localhost", i)) {
            return i;
        };
    }
}

SessionManager::SessionManager() {}

SessionManager::~SessionManager() {
	emit sendKillAll();
}

void SessionManager::startThread(Session* session) {  // ensure thread and object deletion later.
	QThread* thread = new QThread();
	session->moveToThread(thread);
	connect(thread, &QThread::started, session, &Session::onStartSession);
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);
	connect(session, &Session::sendSessionKilled, thread, &QThread::quit);
	connect(session, &Session::sendSessionKilled, session, &Session::deleteLater);
	connect(this, &SessionManager::sendKillAll, session, &Session::deleteLater);

	thread->start();
}

void SessionManager::onStartVideoSession(const QHostAddress ip_address) {
	emit sendStartReciver(ip_address, "video");
}

void SessionManager::onStartAudioSession(const QHostAddress ip_address) {
	emit sendStartReciver(ip_address, "audio");
}

void SessionManager::onKillVideoSession(const QHostAddress ip_address) {
	QString key = "TransmiterVideo";
	if (live_sessions.contains(qMakePair(ip_address, key))) {
		auto it = live_sessions[qMakePair(ip_address, key)];
		QMetaObject::invokeMethod(it.get(), "onKillSession", Qt::QueuedConnection);
		live_sessions.remove(qMakePair(ip_address, key));
	}
}

void SessionManager::onKillAudioSession(const QHostAddress ip_address) {
	QString key = "TransmiterAudio";
	if (live_sessions.contains(qMakePair(ip_address, key))) {
		auto it = live_sessions[qMakePair(ip_address, key)];
		QMetaObject::invokeMethod(it.get(), "onKillSession", Qt::QueuedConnection);
		live_sessions.remove(qMakePair(ip_address, key));
	}
}

void SessionManager::onStartReceivingSession(const QMdnsEngine::Message message_received, const QString session_type) {
	qDebug() << "Starting receiving session" << message_received.address() << " " << session_type;
	const qint16& video_port = 5228;
	const qint16& audio_port = 5229;

	if (session_type == "audio") {
		QPair<QHostAddress, QString> key = qMakePair(message_received.address(), QString("ReciverVideo"));
		auto it = std::make_unique<ReciverAudio>(audio_port);
		startThread(it.get());
		live_sessions.insert(key, std::move(it));
		emit sendSetPorts(message_received, -1, audio_port);  // use -1 for unused ports
	}

	if (session_type == "video") {
		QPair<QHostAddress, QString> key = qMakePair(message_received.address(), QString("ReciverAudio"));
		auto it = std::make_unique<ReciverVideo>(video_port, audio_port);
		startThread(it.get());
		live_sessions.insert(key, std::move(it));
		emit sendSetPorts(message_received, video_port, audio_port);  // use -1 for unused ports
	}
}

void SessionManager::onKillVideoReciver(const QHostAddress ip_address) {
	QString key = "ReciverVideo";
	if (live_sessions.contains(qMakePair(ip_address, key))) {
		auto it = live_sessions[qMakePair(ip_address, key)];
		// it->quit();
		live_sessions.remove(qMakePair(ip_address, key));
	}
}

void SessionManager::onKillAudioReciver(const QHostAddress ip_address) {
	QString key = "ReciverAudio";
	if (live_sessions.contains(qMakePair(ip_address, key))) {
		auto it = live_sessions[qMakePair(ip_address, key)];
		// it->quit();
		live_sessions.remove(qMakePair(ip_address, key));
	}
}

void SessionManager::onReceivedPorts(const QHostAddress ip_address, qint16 video_port, qint16 audio_port) {
	qDebug() << "Starting transmittion session" << ip_address.toString() << " " << video_port << " " << audio_port;
	if (video_port > -1 && audio_port > -1) {
		QPair<QHostAddress, QString> key = qMakePair(QHostAddress(ip_address), QString("TransmiterVideo"));
		auto it = std::make_unique<TransmiterVideo>(ip_address, video_port, audio_port);
		startThread(it.get());
		live_sessions.insert(key, std::move(it));
		return;
	}
	if (video_port > -1) {
		// video only pipeline
		return;
	}
	if (audio_port > -1) {
		QPair<QHostAddress, QString> key = qMakePair(QHostAddress(ip_address), QString("TransmiterAudio"));
		auto it = std::make_unique<TransmiterAudio>(ip_address, 5000);
		startThread(it.get());
		live_sessions.insert(key, std::move(it));
		return;
	}
}
