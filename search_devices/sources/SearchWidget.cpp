#include "SearchWidget.h"

SearchWidget::SearchWidget(QWidget* parent)
	: QListWidget(parent), server(), cache(), mdns_browser(&server, "_mrelay-connect._tcp.local.", &cache), resolver(nullptr) {
	this->setStyleSheet("QListWidget:item { selection-background-color: #00b4d8 }");
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

	ServiceItem* service_item = new ServiceItem(service);
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
		service_item->setResolved();
		qDebug() << address << " resolved.";
		QMdnsEngine::Message message;
		QMdnsEngine::Query query;
		query.setName("mrelay-request-local-ip");
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
	if (queries.front().name() == "mrelay-answer-local-ip.") {
		QString service_name = (++queries.begin())->name();
		QString local_ip = queries.back().name().left(queries.back().name().size() - 1);
		service_name = service_name.left(service_name.size() - 1);
		qDebug() << "got requested local ip from: " << service_name << "which is: " << local_ip;
		ServiceItem* service_item = service_item_map[service_name];
		service_item->setLocalIP(local_ip);
	}
}

void SearchWidget::onAddButtonCLicked() {
	QList<QListWidgetItem*> selected_items = this->selectedItems();	 // Only one can actually be selected at a time.
	if (!selected_items.empty()) {
		ServiceItem* service_item = dynamic_cast<ServiceItem*>(selected_items.front());
		emit devicePreparedToAdd(service_item->getService().name(), service_item->getLocalIP());
	} else {
		qDebug() << "Device selection empty.";
	}
}
