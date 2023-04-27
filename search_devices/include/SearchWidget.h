#pragma once
// Qt basic libraries
#include <QGridLayout>
#include <QListWidget>
#include <QMap>
#include <QString>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QHostAddress>
#include <QWidget>
// qmdnsengine
#include <qmdnsengine/browser.h>
#include <qmdnsengine/cache.h>
#include <qmdnsengine/hostname.h>
#include <qmdnsengine/message.h>
#include <qmdnsengine/query.h>
#include <qmdnsengine/record.h>
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

	ServiceItem* selected_item;
	QMap<QString, ServiceItem*> service_widgets_map;

public:
	SearchWidget(QWidget* parent = nullptr);

private slots:
	void onDiscovered(const QMdnsEngine::Service& service);

	void onServiceRemoved(const QMdnsEngine::Service& service);

	void onServiceUpdated(const QMdnsEngine::Service& service);

	void onSelected(QListWidgetItem* item);

	void onResolved(const QHostAddress& address);

	void onMessageReceived(const QMdnsEngine::Message& message_received);
};
