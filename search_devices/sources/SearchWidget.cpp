#include "SearchWidget.h"

#include <networkTools.h>
#include <QScrollBar>
#include <UITools.h>

SearchWidget::SearchWidget(qreal scale_, QWidget* parent)
	: QListWidget(parent),
	  server(),
	  cache(),
	  mdns_browser(&server, "_mrelay-connect._tcp.local.", &cache),
	  resolver(nullptr),
	  scale(scale_) {
	QFont font = this->font();
	font.setPointSize(16 * getFontScaling(scale));
	this->setFont(font);
	this->setStyleSheet("QListWidget:item { selection-background-color: #bbbbff }");
	this->setSelectionBehavior(QAbstractItemView::SelectItems);
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(&mdns_browser, &QMdnsEngine::Browser::serviceAdded, this, &SearchWidget::onDiscovered);
	connect(&mdns_browser, &QMdnsEngine::Browser::serviceRemoved, this, &SearchWidget::onServiceRemoved);
	connect(&mdns_browser, &QMdnsEngine::Browser::serviceUpdated, this, &SearchWidget::onServiceUpdated);
	connect(&server, &QMdnsEngine::Server::messageReceived, this, &SearchWidget::onMessageReceived);
	connect(this, &QListWidget::itemClicked, this, &SearchWidget::onSelected);
}

void SearchWidget::onDiscovered(const QMdnsEngine::Service& service) {
	qDebug() << service.name() << "discovered!";
	if (QString(service.type()) != QString("_mrelay-connect._tcp.local.") || service_item_map.contains(getServiceName(service))) {
		return;
	}

	ServiceItem* service_item = new ServiceItem(service, scale);
	service_item_map[getServiceName(service)] = service_item;
	this->addItem(service_item);
	this->setItemWidget(service_item, service_item->getWidget());
	//	connect(service_item->getAddButton(), QPushButton::clicked(), this, &SearchWidget::onAddButtonClicked);
}

void SearchWidget::onServiceRemoved(const QMdnsEngine::Service& service) {
	if (QString(service.type()) != QString("_mrelay-connect._tcp.local.") || !service_item_map.contains(getServiceName(service))) {
		return;
	}
	qDebug() << service.name() << "removing!";
	ServiceItem* service_item = service_item_map[getServiceName(service)];
	service_item_map.remove(getServiceName(service));
	delete service_item;
}

void SearchWidget::onServiceUpdated(
	const QMdnsEngine::Service& service) {	// No updated signals for some reason. Handling with discovered and removed currently.
	if (QString(service.type()) != QString("_mrelay-connect._tcp.local.")) {
		return;
	}
	ServiceItem* service_item = service_item_map[getServiceName(service)];
	service_item->update(service);
	qDebug() << service.name() << "updated!";
}

void SearchWidget::onSelected(QListWidgetItem* item_) {
	ServiceItem* service_item = dynamic_cast<ServiceItem*>(item_);
	if (resolver) {
		delete resolver;
		resolver = nullptr;
	}
	resolver = new QMdnsEngine::Resolver(&server, service_item->getService().hostname(), &cache, this);
	connect(resolver, &QMdnsEngine::Resolver::resolved, this, [this, service_item](const QHostAddress& address) {
		if (address.protocol() != QAbstractSocket::IPv6Protocol) {
			return;
		}
		if (service_item->getResolved()) {
			return;
		}
		qDebug() << "resolved address: " << address.toString();
		service_item->setResolved(address);
		QMdnsEngine::Message message;
		QMdnsEngine::Query query;
		query.setName("mrelay-request-mac-address");
		query.setType(2222);
		message.addQuery(query);
		query.setName(getServiceName(service_item->getService()).toUtf8());
		query.setType(2222);
		message.addQuery(query);
		message.setAddress(address);
		message.setPort(5353);
		message.setTransactionId(1264);
		server.sendMessage(message);
	});
}

void SearchWidget::onMessageReceived(const QMdnsEngine::Message& message_received) {
	if (message_received.transactionId() != 3663) {
		return;
	}
	QList<QMdnsEngine::Query> queries = message_received.queries();
	if (queries[0].name() == "mrelay-answer-mac-address.") {
		QString service_name = queries[1].name().left(queries[1].name().size() - 1);
		QString mac_address = queries[2].name().left(queries[2].name().size() - 1);
		ServiceItem* service_item = service_item_map[service_name];
		qDebug() << "got requested mac address from: " << service_name << "which is: " << mac_address;
		service_item->setMacAddress(mac_address);
		if (device_ids.contains(mac_address)) {
			qDebug() << "Service item already added. Updating address.";
			service_item->setAlreadyAdded();
			emit sendUpdateAddress(mac_address, message_received.address());
			return;
		}
	}
}

void SearchWidget::onAddButtonCLicked() {
	QList<QListWidgetItem*> selected_items = this->selectedItems();	 // Only one can actually be selected at a time.
	if (selected_items.empty()) {
		qDebug() << "Can't add device. Device selection empty.";
		return;
	}
	ServiceItem* service_item = dynamic_cast<ServiceItem*>(selected_items.front());
	if (!service_item->getResolved()) {
		qDebug() << "Can't add device. Not yet resolved.";
		return;
	}
	if (service_item->getAdded()) {	 // still can add if service name changed.
		qDebug() << "Can't add device. Device already added.";
		return;
	}

	service_item->setAlreadyAdded();
	emit devicePreparedToAdd(service_item->getService().name(), service_item->getIPv6Address(), service_item->getMacAddress());
}

void SearchWidget::onDeviceIdsUpdated(QSet<QString> device_ids_) {
	device_ids = std::move(device_ids_);
}

void SearchWidget::onStartReciver(const QHostAddress& local_ip6, const QString& session_type) {
	qDebug() << "sending receive request to: " << QHostAddress(local_ip6).toString() << " from: " << getIPv6();
	QMdnsEngine::Message message;
	QMdnsEngine::Query query;
	query.setName("mrelay-start-receiving-session");
	query.setType(7575);
	message.addQuery(query);
	query.setName(session_type.toUtf8());
	query.setType(7575);
	message.addQuery(query);
	message.setAddress(local_ip6);
	message.setPort(5353);
	message.setTransactionId(3645);
	server.sendMessage(message);
}
