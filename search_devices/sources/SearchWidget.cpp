#include "SearchWidget.h"

#include <UITools.h>
#include <networkTools.h>

#include <QScrollBar>

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
    connect(this, &QListWidget::itemClicked, this, &SearchWidget::onSelected);
}

void SearchWidget::onDiscovered(const QMdnsEngine::Service service) {
    qDebug() << service.name() << "discovered!";
    if (QString(service.type()) != QString("_mrelay-connect._tcp.local.") || service_item_map.contains(getServiceName(service))) {
        return;
    }
    ServiceItem* service_item = new ServiceItem(service, scale);
    service_item_map[getServiceName(service)] = service_item;
    this->addItem(service_item);
    this->setItemWidget(service_item, service_item->getWidget());
    QMdnsEngine::Resolver* resolver = new QMdnsEngine::Resolver(&server, service_item->getService().hostname(), &cache, this);
    connect(resolver, &QMdnsEngine::Resolver::resolved, this, [this, service_item, resolver](const QHostAddress& address) {
        if (address.protocol() != QAbstractSocket::IPv4Protocol) {
            return;
        }
        if (service_item->getResolved()) {
            return;
        }
        qDebug() << "resolved address: " << address.toString();
        service_item->setResolved(address);
        emit sendRequestInitialization(address, getServiceName(service_item->getService()).toUtf8());

        QMetaObject::invokeMethod(resolver, &QMdnsEngine::Resolver::deleteLater);
    });
}

void SearchWidget::onInitializationResponse(QString service_name, QString mac_address) {
    ServiceItem* service_item = service_item_map[service_name];
    qDebug() << "got mrelay-answer-initialization from: " << service_name;
    service_item->setInitialized(mac_address);
    if (device_ids.contains(mac_address)) {
        qDebug() << "Service item already added. Updating address.";
        service_item->setAlreadyAdded();
        emit sendUpdateAddress(mac_address, service_item->getLocalIPv4Address());
        return;
    }
}

void SearchWidget::onServiceRemoved(const QMdnsEngine::Service service) {
    if (QString(service.type()) != QString("_mrelay-connect._tcp.local.") || !service_item_map.contains(getServiceName(service))) {
        return;
    }
    qDebug() << service.name() << "removing!";
    ServiceItem* service_item = service_item_map[getServiceName(service)];
    emit sendUnsetAddress(service_item_map[getServiceName(service)]->getMacAddress());
    service_item_map.remove(getServiceName(service));
    delete service_item;
}
// No updated signals for some reason. Handling with discovered and removed currently.
void SearchWidget::onServiceUpdated(QMdnsEngine::Service service) {
    if (QString(service.type()) != QString("_mrelay-connect._tcp.local.")) {
        return;
    }
    ServiceItem* service_item = service_item_map[getServiceName(service)];
    service_item->update(service);
    qDebug() << service.name() << "updated!";
}

void SearchWidget::onSelected(QListWidgetItem* item_) {}

void SearchWidget::onAddButtonCLicked() {
    QList<QListWidgetItem*> selected_items = this->selectedItems();  // Only one can actually be selected at a time.
    if (selected_items.empty()) {
        qDebug() << "Can't add device. Device selection empty.";
        return;
    }
    ServiceItem* service_item = dynamic_cast<ServiceItem*>(selected_items.front());
    if (!service_item->getResolved()) {
        qDebug() << "Can't add device. Not yet resolved.";
        return;
    }
    if (!service_item->getInitialized()) {
        qDebug() << "Can't add device. Not yet initialized.";
        return;
    }
    if (service_item->getAdded()) {  // still can add if service name changed.
        qDebug() << "Can't add device. Device already added.";
        return;
    }
    service_item->setAlreadyAdded();
    emit sendDevicePreparedToAdd(service_item->getService().name(), service_item->getLocalIPv4Address(), service_item->getMacAddress());
}

void SearchWidget::onDeviceIdsUpdated(QSet<QString> device_ids_) {
    device_ids = std::move(device_ids_);
}
