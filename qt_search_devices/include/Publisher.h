#pragma once
// Qt basic libraries.
#include <QWidget>
// qmdnsengine
#include <qmdnsengine/hostname.h>
#include <qmdnsengine/provider.h>
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
	Publisher(QWidget* parent = nullptr);

private slots:

	void onHostnameChanged(const QByteArray& hostname);
};
