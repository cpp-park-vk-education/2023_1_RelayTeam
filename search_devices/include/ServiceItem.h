#pragma once
// Qt basic libraries
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPixmap>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <QtNetwork/QHostAddress>
// qmdnsengine
#include <qmdnsengine/service.h>

class ServiceItem : public QListWidgetItem {
private:
    QLabel* service_name_label;
    QLabel* connection_status_icon;
    QLabel* os_icon_label;
    QLabel* address_label;
    QHBoxLayout* main_layout;
    QVBoxLayout* name_layout;
    QPushButton* add_button;
    QWidget* main_widget;

    QMdnsEngine::Service service;
    bool is_added;
    bool is_resolved;
    bool is_initialized;
    QString mac_address;
    QHostAddress local_ipv4_address;

public:
    ServiceItem(const QMdnsEngine::Service& service_, qreal scale);

    ~ServiceItem() {}

    void update(const QMdnsEngine::Service& service_);

    inline QWidget* getWidget() {
        return main_widget;
    }

    inline QMdnsEngine::Service getService() {
        return service;
    }

    inline void setResolved(const QHostAddress& local_ipv4_address_) {
        is_resolved = true;
        local_ipv4_address = local_ipv4_address_;
        address_label = new QLabel(local_ipv4_address_.toString());
        address_label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        name_layout->addWidget(address_label);
        connection_status_icon->setStyleSheet("QLabel {background-color : yellow}");
    }

    inline QHostAddress getLocalIPv4Address() {
        return local_ipv4_address;
    }

    inline bool getResolved() {
        return is_resolved;
    }

    inline bool getAdded() {
        return is_added;
    }

    inline bool getInitialized() {
        return is_initialized;
    }

    inline void setAlreadyAdded() {
        is_added = true;
        connection_status_icon->setStyleSheet("QLabel {background-color : blue}");
    }

    inline void setInitialized(const QString& mac_address_) {
        is_initialized = true;
        mac_address = mac_address_;
        connection_status_icon->setStyleSheet("QLabel {background-color : green}");
    }

    inline QString getMacAddress() {
        return mac_address;
    }
};
