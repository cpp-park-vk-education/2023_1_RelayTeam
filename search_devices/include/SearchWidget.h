#pragma once
// Qt basic libraries
#include <QGridLayout>
#include <QListWidget>
#include <QString>
#include <QtNetwork/QHostAddress>
#include <QWidget>
// qmdnsengine
#include <qmdnsengine/browser.h>
#include <qmdnsengine/cache.h>
#include <qmdnsengine/hostname.h>
#include <qmdnsengine/resolver.h>
#include <qmdnsengine/server.h>
#include <qmdnsengine/service.h>

#include "ServiceWidget.h"

class SearchWidget : public QWidget {
private:
	Q_OBJECT
	QMdnsEngine::Server server;
	QMdnsEngine::Cache cache;
	QMdnsEngine::Browser mdns_browser;
	QListWidget* service_list;
	QGridLayout* main_layout;
	QMdnsEngine::Resolver* resolver;

public:
	SearchWidget(QWidget* parent = nullptr);

private slots:
	void onDiscovered(const QMdnsEngine::Service& service);

	void onResolved(const QHostAddress& address);

	void onServiceRemoved(const QMdnsEngine::Service& service);

	void onServiceUpdated(const QMdnsEngine::Service& service);

	void onSelected(ServiceWidget* service);
};
