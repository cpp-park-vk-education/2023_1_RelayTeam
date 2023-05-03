#pragma once
// Qt basic libraries
#include <QGridLayout>
#include <QListWidget>
#include <QMap>
#include <QString>
#include <QWidget>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QHostAddress>
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

#include "ServiceItem.h"

class SearchWidget : public QListWidget {
private:
	Q_OBJECT
	QMdnsEngine::Server server;
	QMdnsEngine::Cache cache;
	QMdnsEngine::Browser mdns_browser;
	QMdnsEngine::Resolver* resolver;
	QSet<QString> device_ids;

	qreal scale;
	QMap<QString, ServiceItem*> service_item_map;

	inline QString getServiceName(const QMdnsEngine::Service& service) {
		return service.hostname() + service.name();
	}

public:
	SearchWidget(qreal scale_, QWidget* parent = nullptr);

private slots:
	void onDiscovered(QMdnsEngine::Service service);

	void onServiceRemoved(QMdnsEngine::Service service);

	void onServiceUpdated(QMdnsEngine::Service service);

	void onSelected(QListWidgetItem* item);

	void onMessageReceived(QMdnsEngine::Message message_received);

public slots:
	void onAddButtonCLicked();

	void onDeviceIdsUpdated(QSet<QString> device_ids_);

	void onStartReciver(const QHostAddress ip_address, const QString session_type);

signals:
	void devicePreparedToAdd(QString name, QHostAddress ipv6_address, QString mac_address);

	void sendUpdateAddress(QString mac_address, QHostAddress local_ipv4_address);

	void sendReceivedPorts(const QHostAddress ipv6_address, qint32 video_port, qint32 audio_port);
};
