#include "SearchWidget.h"

SearchWidget::SearchWidget(QWidget* parent)
	: QWidget(parent),
	  server(),
	  cache(),
	  mdns_browser(&server, "_mrelay-connect._tcp.local.", &cache),
	  resolver(nullptr),
	  selected_item(nullptr) {
	main_layout = new QGridLayout();
	this->setLayout(main_layout);
	service_list = new QListWidget();

	main_layout->addWidget(static_cast<QWidget*>(service_list), 0, 0);

	connect(&mdns_browser, &QMdnsEngine::Browser::serviceAdded, this, &SearchWidget::onDiscovered);
	connect(&server, &QMdnsEngine::Server::messageReceived, this, &SearchWidget::onMessageReceived);
}

void SearchWidget::onDiscovered(const QMdnsEngine::Service& service) {
	if (QString(service.type()) != QString("_mrelay-connect._tcp.local.")) {
		return;
	}
	qDebug() << service.name() << "discovered!";

	ServiceItem* service_item = new ServiceItem(service);

	service_widgets_map[service.hostname()] = service_item;
	service_list->addItem(service_item);
	service_list->setItemWidget(service_item, service_item->getWidget());
	connect(service_list, &QListWidget::itemClicked, this, &SearchWidget::onSelected);
}

void SearchWidget::onServiceRemoved(const QMdnsEngine::Service& service) {
	if (QString(service.type()) != QString("_mrelay-connect._tcp.local.")) {
		return;
	}
	qDebug() << service.name() << "removing!";
	ServiceItem* service_item = service_widgets_map[service.hostname()];
	delete service_item;
}

void SearchWidget::onServiceUpdated(const QMdnsEngine::Service& service) {
	if (QString(service.type()) != QString("_mrelay-connect._tcp.local.")) {
		return;
	}
	ServiceItem* service_item = service_widgets_map[service.hostname()];
	service_item->update(service);
	qDebug() << service.name() << "updated!";
}

void SearchWidget::onSelected(QListWidgetItem* item_) {
	if (selected_item) {
		selected_item->setUnResolved();
	}
	ServiceItem* item = dynamic_cast<ServiceItem*>(item_);
	selected_item = item;
	if (resolver) {
		delete resolver;
		resolver = nullptr;
	}
	resolver = new QMdnsEngine::Resolver(&server, item->getService().hostname(), &cache, this);
	connect(resolver, &QMdnsEngine::Resolver::resolved, this, &SearchWidget::onResolved);
}

void SearchWidget::onResolved(const QHostAddress& address) {
	if (address.protocol() == QAbstractSocket::IPv4Protocol) {
		selected_item->setResolved();
		qDebug() << address << " resolved.";
		QMdnsEngine::Message message;  // should pass item id with message!!!
		message.setAddress(address);
		message.setPort(5353);
		QMdnsEngine::Query query;
		query.setName(QString("request_local_ip").toUtf8());
		query.setType(100);
		message.addQuery(query);
		message.setTransactionId(1264);
		server.sendMessage(message);
	}
}

void SearchWidget::onMessageReceived(const QMdnsEngine::Message& message_received) {
	if (message_received.transactionId() != 3663) {
		return;
	}
	if (message_received.queries().size() != 1) {
		return;
	}
	if (message_received.queries().first().type() == 8234) {
		selected_item->setGotLocalIP();
		qDebug() << "got requested local ip from: " << message_received.address();
		qDebug() << "which is: " << message_received.queries().first().name();
	}
}
