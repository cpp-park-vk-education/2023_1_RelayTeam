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
    } else if (session_type == "camera") {
        connect(thread, &QThread::started, session, &Session::onEbableCamera);
    }
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(session, &Session::sendSessionKilled, thread, &QThread::quit);
    connect(session, &Session::sendSessionKilled, session, &Session::deleteLater);
    connect(this, &SessionManager::setVolume, static_cast<Reciver*>(session), &Reciver::onSetVolume);
    connect(this, &SessionManager::setBitrate, static_cast<Transmiter*>(session), &Transmiter::onSetBitrate);
    connect(this, &SessionManager::sendKillAll, session, &Session::deleteLater);
    connect(this, &SessionManager::setPorts, this, &SessionManager::onReceivedCameraPorts);
    connect(session, &Session::closeWindow, this, &SessionManager::onCloseWindow);
    connect(static_cast<Transmiter*>(session), &Transmiter::closeWindow, this, &SessionManager::sendVideoSessionKilled);

    thread->start();
}

void SessionManager::killSessionIp4(const QHostAddress ip_address, const QString key)
{
  if (live_sessions.contains(qMakePair(QHostAddress(ip_address.toIPv4Address()), key))) {
      auto it = live_sessions[qMakePair(QHostAddress(ip_address.toIPv4Address()), key)];
      QMetaObject::invokeMethod(it.get(), "onKillSession", Qt::QueuedConnection);
      live_sessions.remove(qMakePair(QHostAddress(ip_address.toIPv4Address()), key));
      qDebug() << "onKillSession" << ip_address << key;
  }

}

void SessionManager::killSessionIp6(const QHostAddress ip_address, const QString key)
{
  if (live_sessions.contains(qMakePair(QHostAddress(ip_address.toIPv6Address()), key))) {
      auto it = live_sessions[qMakePair(QHostAddress(ip_address.toIPv6Address()), key)];
      QMetaObject::invokeMethod(it.get(), "onKillSession", Qt::QueuedConnection);
      live_sessions.remove(qMakePair(QHostAddress(ip_address.toIPv6Address()), key));
      qDebug() << "onKillSession" << QHostAddress(ip_address.toIPv6Address()) << key;
  }
}

//void SessionManager::insertSession(const QHostAddress ip_address, const QString session_type, qint32 video_port, qint32 audio_port)
//{
//  QPair<QHostAddress, QString> keyTransmiter = qMakePair(QHostAddress(ip_address), QString(session_type));
//  auto itTransmiter = std::make_unique<Transmiter>(ip_address, video_port, audio_port);
//  startThread(itTransmiter.get(), "video");
//  live_sessions.insert(keyTransmiter, std::move(itTransmiter));
//}


void SessionManager::onStartVideoSession(const QHostAddress ip_address) {
    emit sendStartReciver(ip_address, "video");
}

void SessionManager::onStartCameraSession(const QHostAddress ip_address) {
    emit sendStartReciver(ip_address, "camera");
}

void SessionManager::onStartAudioSession(const QHostAddress ip_address) {
    emit sendStartReciver(ip_address, "audio");
}

void SessionManager::onKillVideoSession(const QHostAddress ip_address) {
    QString transKey = "TransmiterVideo";
    QString recKey = "ReciverVideo";

    killSessionIp4(ip_address, transKey);
    killSessionIp6(ip_address, recKey);
    onKillCameraSession(ip_address);
}

void SessionManager::onKillAudioSession(const QHostAddress ip_address) {
    QString key = "TransmiterAudio";
    killSessionIp4(ip_address, key);

    QString recAudioKey = "ReciverAudio";
    killSessionIp6(ip_address, recAudioKey);
}

void SessionManager::onKillCameraSession(const QHostAddress ip_address) {
    QString recKeyCam = "ReciverCamera";
    QString transKeyCam = "TransmiterCamera";

    killSessionIp4(ip_address, recKeyCam);
    killSessionIp6(ip_address, recKeyCam);

    killSessionIp4(ip_address, transKeyCam);
    killSessionIp6(ip_address, transKeyCam);
}

void SessionManager::onReceivedPorts(const QHostAddress ip_address, qint32 video_port, qint32 audio_port) {
    qDebug() << "Starting transmittion session" << ip_address.toString() << " " << video_port << " " << audio_port;
    if (video_port != -1) {
        QPair<QHostAddress, QString> keyTransmiter = qMakePair(QHostAddress(ip_address), QString("TransmiterVideo"));
        auto itTransmiter = std::make_unique<Transmiter>(ip_address, video_port, audio_port);
        qDebug() << "VIDEO_PORT" << video_port;
        startThread(itTransmiter.get(), "video");
        live_sessions.insert(keyTransmiter, std::move(itTransmiter));
        return;
    }

    if (audio_port != -1) {
        QPair<QHostAddress, QString> key = qMakePair(QHostAddress(ip_address), QString("TransmiterAudio"));
        auto it = std::make_unique<Transmiter>(ip_address, audio_port);
        qDebug() << "AUDIO_PORT" << audio_port;
        startThread(it.get(), "audio");
        live_sessions.insert(key, std::move(it));
        return;
    }

    if (audio_port == -1 && video_port == -1) {
         QPair<QHostAddress, QString> keyTransmiter = qMakePair(ip_address, QString("TransmiterCamera"));
        auto itTransmiter = std::make_unique<Transmiter>(ip_address, video_port, -1);
        qDebug() << "CAMERA_PORT" << video_port;
        startThread(itTransmiter.get(), "camera");
        live_sessions.insert(keyTransmiter, std::move(itTransmiter));
        return;
    }
}

void SessionManager::onReceivedCameraPorts(const QHostAddress ip_address, qint32 video_port) {
    QPair<QHostAddress, QString> keyTransmiter = qMakePair(ip_address, QString("TransmiterCamera"));
    auto itTransmiter = std::make_unique<Transmiter>(ip_address, video_port, -1);
    qDebug() << "CAMERA_PORT" << video_port;
    startThread(itTransmiter.get(), "camera");
    live_sessions.insert(keyTransmiter, std::move(itTransmiter));
}

void SessionManager::onStartReceivingSession(const QHostAddress ip_address, const QString session_type) {
    qDebug() << "Starting receiving session" << ip_address << " " << session_type;
    const qint16& video_port = 5228;
    const qint16& audio_port = 5229;

    if (session_type == "audio") {
        QPair<QHostAddress, QString> key = qMakePair(ip_address, QString("ReciverAudio"));
        auto it = std::make_unique<Reciver>(ip_address, audio_port);
        startThread(it.get(), "audio");
        live_sessions.insert(key, std::move(it));
        emit sendSetPorts(ip_address, -1, audio_port);  // use -1 for unused ports
    }

    if (session_type == "video") {
        QPair<QHostAddress, QString> key = qMakePair(ip_address, QString("ReciverVideo"));
        auto it = std::make_unique<Reciver>(ip_address, video_port, audio_port);
        startThread(it.get(), "video");
        live_sessions.insert(key, std::move(it));
        emit sendSetPorts(ip_address, video_port, -1);  // use -1 for unused ports
    }

    if (session_type == "camera") {
        QPair<QHostAddress, QString> key = qMakePair(ip_address, QString("ReciverCamera"));
        auto it = std::make_unique<Reciver>(ip_address, video_port);
        startThread(it.get(), "camera");
        live_sessions.insert(key, std::move(it));
        emit sendSetPorts(ip_address, -1, -1);  // use -1 for unused ports
    }
}

void SessionManager::onCloseWindow(const QHostAddress& ip_address_, const QString session_type) {
    qDebug() << "Window closing" << ip_address_ << "session_type: " << session_type;

    if (session_type == "video") {
        onKillVideoSession(ip_address_);

    }
    else if (session_type == "camera") {
        onKillCameraSession(ip_address_);
    }
}

void SessionManager::onSetVolume(const QHostAddress ip_address, const int volume_) {
    float volume = volume_ / 100.0f;
    if (volume == 1) {
        volume = 0.99;
    }
    QString key = "ReciverAudio" ;

    if (live_sessions.contains(qMakePair(QHostAddress(ip_address.toIPv6Address()), key))) {
        auto it = live_sessions[qMakePair(QHostAddress(ip_address.toIPv6Address()), key)];
        QMetaObject::invokeMethod(it.get(), "onSetVolume", Qt::DirectConnection, Q_ARG(float, volume));
    }
}

void SessionManager::onSetBitrate(const QHostAddress ip_address, const int bitrate) {
    QString key = "TransmiterVideo";

    qDebug() << "Set bitrate" << bitrate;
    if (live_sessions.contains(qMakePair(ip_address, key))) {
        auto it = live_sessions[qMakePair(ip_address, key)];
        QMetaObject::invokeMethod(it.get(), "onSetBitrate", Qt::DirectConnection, Q_ARG(int, bitrate));
    }
}

void SessionManager::setCameraCaptureMode(const QHostAddress ip_address)
{
  qDebug() << "Set camera capture mode";

  if (live_sessions.contains(qMakePair(QHostAddress(ip_address.toIPv6Address()),  QString("ReciverCamera"))) || live_sessions.contains(qMakePair(ip_address,  QString("TransmiterCamera")))){
    onKillCameraSession(ip_address);
    onKillVideoSession(ip_address);

    emit sendStartReciver(ip_address, "video");
  }

}

void SessionManager::setScreenCaptureMode(const QHostAddress ip_address)
{
  qDebug() << "Set screen capture mode";

  if (live_sessions.contains(qMakePair(QHostAddress(ip_address.toIPv6Address()),  QString("ReciverVideo"))) || live_sessions.contains(qMakePair(ip_address,  QString("TransmiterVideo")))){
    onKillVideoSession(ip_address);
    onKillCameraSession(ip_address);
    emit sendStartReciver(ip_address, "camera");
  }

}
