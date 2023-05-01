#pragma once
// Qt basic libraries.
#include <QList>
#include <QString>
#include <QtNetwork/QHostInfo>
#include <QWidget>
// qmdnsengine
#include <qmdnsengine/hostname.h>
#include <qmdnsengine/message.h>
#include <qmdnsengine/provider.h>
#include <qmdnsengine/query.h>
#include <qmdnsengine/record.h>
#include <qmdnsengine/server.h>
#include <qmdnsengine/service.h>

class Publisher : public QObject {
private:
	Q_OBJECT
	QMdnsEngine::Server server;
	QMdnsEngine::Hostname hostname;
	QMdnsEngine::Provider provider;
	QMdnsEngine::Service service;

public:
	Publisher(const QString& device_name, QWidget* parent = nullptr);

private slots:
	void onHostnameChanged(const QByteArray& hostname);

	void onMessageReceived(const QMdnsEngine::Message& message_received);

public slots:
	void onChangeServiceName(const QString& service_name);

	void onSendPorts(const QMdnsEngine::Message& message_received, qint16 video_port, qint16 audio_port);

signals:
	void sendStartReceivingSession(const QMdnsEngine::Message& message_received, QString session_type);
};
