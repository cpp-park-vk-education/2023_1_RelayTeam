#include "SearchWidget.h"

SearchWidget::SearchWidget(QWidget* parent)
	: QWidget(parent), server(), cache(), mdns_browser(&server, "_mrelay-connect._tcp.local.", &cache), resolver(nullptr) {
	;
	main_layout = new QGridLayout();
	this->setLayout(main_layout);
	service_list = new QListWidget();

	main_layout->addWidget(static_cast<QWidget*>(service_list), 0, 0);

	connect(&mdns_browser, &QMdnsEngine::Browser::serviceAdded, this, &SearchWidget::onDiscovered);
}

void SearchWidget::onDiscovered(const QMdnsEngine::Service& service) {
	if (QString(service.type()) != QString("_mrelay-connect._tcp.local.")) {
		return;
	}
	qDebug() << service.name() << "discovered!";
	if (resolver) {
		delete resolver;
		resolver = nullptr;
	}

	resolver = new QMdnsEngine::Resolver(&server, service.hostname(), &cache, this);
	connect(resolver, &QMdnsEngine::Resolver::resolved, this, &SearchWidget::onResolved);

	QListWidgetItem* item = new QListWidgetItem();
	ServiceWidget* service_widget = new ServiceWidget(service, item);

	service_list->addItem(item);
	service_list->setItemWidget(item, static_cast<QWidget*>(service_widget));
}

void SearchWidget::onServiceRemoved(const QMdnsEngine::Service& service) {
	qDebug() << service.name() << "removed!";
}

void SearchWidget::onServiceUpdated(const QMdnsEngine::Service& service) {
	qDebug() << service.name() << "updated!";
}

void SearchWidget::onResolved(const QHostAddress& address) {
	qDebug() << "resolved to" << address;
}

void SearchWidget::onSelected(ServiceWidget* service) {}
