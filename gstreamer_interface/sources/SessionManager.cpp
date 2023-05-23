#include "SessionManager.h"

#include <networkTools.h>

#include "qobjectdefs.h"

SessionManager::SessionManager() {}

SessionManager::~SessionManager() {
    emit sendKillAll();
}

void SessionManager::startThread(Session* session, const QString session_type) {  // ensure thread and object deletion later.
    QThread* thread = new QThread();
    session->moveToThread(thread);
    if (session_type == "video") {
        connect(thread, &QThread::started, session, &Session::onEnableVideo);
    } else if (session_type == "audio") {
        connect(thread, &QThread::started, session, &Session::onEnableAudio);
    }
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(session, &Session::sendSessionKilled, thread, &QThread::quit);
    connect(session, &Session::sendSessionKilled, session, &Session::deleteLater);
    connect(this, &SessionManager::setVolume, session, &Session::onSetVolume);
    connect(this, &SessionManager::setBitrate, session, &Session::onSetBitrate);
    connect(this, &SessionManager::sendKillAll, session, &Session::deleteLater);

    thread->start();
}

void SessionManager::onStartVideoSession(const QHostAddress ip_address) {
    emit sendStartReciver(ip_address, "video");

    onSetVolume(ip_address, 0);
}

void SessionManager::onStartAudioSession(const QHostAddress ip_address) {
    emit sendStartReciver(ip_address, "audio");
    // onSetBitrate(ip_address, 800);
}

void SessionManager::onKillVideoSession(const QHostAddress ip_address) {
    QString transKey = "TransmiterVideo";
    QString recKey = "ReciverVideo";
    if (live_sessions.contains(qMakePair(ip_address, transKey))) {
        auto it = live_sessions[qMakePair(ip_address, transKey)];
        QMetaObject::invokeMethod(it.get(), "onKillSession", Qt::QueuedConnection);
        live_sessions.remove(qMakePair(ip_address, transKey));
    }
    if (live_sessions.contains(qMakePair(ip_address, recKey))) {
        auto it = live_sessions[qMakePair(ip_address, recKey)];
        QMetaObject::invokeMethod(it.get(), "onKillSession", Qt::QueuedConnection);
        live_sessions.remove(qMakePair(ip_address, recKey));
    }
}

void SessionManager::onKillAudioSession(const QHostAddress ip_address) {
    QString key = "TransmiterAudio";
    if (live_sessions.contains(qMakePair(ip_address, key))) {
        auto it = live_sessions[qMakePair(ip_address, key)];
        QMetaObject::invokeMethod(it.get(), "onKillSession", Qt::QueuedConnection);
        live_sessions.remove(qMakePair(ip_address, key));
    }

    QString recAudioKey = "ReciverAudio";
    if (live_sessions.contains(qMakePair(ip_address, recAudioKey))) {
        auto it = live_sessions[qMakePair(ip_address, recAudioKey)];
        QMetaObject::invokeMethod(it.get(), "onKillSession", Qt::QueuedConnection);
        live_sessions.remove(qMakePair(ip_address, recAudioKey));
    }
}

void SessionManager::onStartReceivingSession(const QHostAddress ip_address, const QString session_type) {
    qDebug() << "Starting receiving session" << ip_address << " " << session_type;
    const qint16& video_port = 5228;
    const qint16& audio_port = 5229;

    if (session_type == "audio") {
        QPair<QHostAddress, QString> key = qMakePair(ip_address, QString("ReciverAudio"));
        auto it = std::make_unique<Reciver>(audio_port);
        startThread(it.get(), "audio");
        live_sessions.insert(key, std::move(it));
        emit sendSetPorts(ip_address, -1, audio_port);  // use -1 for unused ports
    }

    if (session_type == "video") {
        QPair<QHostAddress, QString> key = qMakePair(ip_address, QString("ReciverVideo"));
        auto it = std::make_unique<Reciver>(video_port, audio_port);
        startThread(it.get(), "video");
        live_sessions.insert(key, std::move(it));
        emit sendSetPorts(ip_address, video_port, -1);  // use -1 for unused ports
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

void SessionManager::onReceivedPorts(const QHostAddress ip_address, qint32 video_port, qint32 audio_port) {
    qDebug() << "Starting transmittion session" << ip_address.toString() << " " << video_port << " " << audio_port;
    if (video_port != 65535) {
        QString keyReciver = "ReciverVideo";
        QPair<QHostAddress, QString> keyTransmiter = qMakePair(QHostAddress(ip_address), QString("TransmiterVideo"));
        auto itTransmiter = std::make_unique<Transmiter>(ip_address, video_port, audio_port);
        qDebug() << "VIDEO_PORT";
        startThread(itTransmiter.get(), "video");
        live_sessions.insert(keyTransmiter, std::move(itTransmiter));
        return;
    }
    if (audio_port != 65535) {
        QPair<QHostAddress, QString> key = qMakePair(QHostAddress(ip_address), QString("TransmiterAudio"));
        auto it = std::make_unique<Transmiter>(ip_address, audio_port);
        qDebug() << "AUDIO_PORT";
        startThread(it.get(), "audio");
        live_sessions.insert(key, std::move(it));
        return;
    }
}

void SessionManager::onSetVolume(const QHostAddress ip_address, const int volume) {
    QString key = "ReciverAudio";
    if (live_sessions.contains(qMakePair(ip_address, key))) {
        auto it = live_sessions[qMakePair(ip_address, key)];
        // it->onSetVolume(volume);
        // emit setVolume(volume);
        QMetaObject::invokeMethod(it.get(), "onSetVolume", Qt::DirectConnection, Q_ARG(float, volume));
    }
}

void SessionManager::onSetBitrate(const QHostAddress ip_address, const int bitrate) {
    QString key = "TransmiterVideo";
    if (live_sessions.contains(qMakePair(ip_address, key))) {
        auto it = live_sessions[qMakePair(ip_address, key)];
        emit setBitrate(bitrate);
        // QMetaObject::invokeMethod(it.get(), "onSetVolume", Qt::QueuedConnection, a);
        // it->onSetBitrate(bitrate);
    }
}
