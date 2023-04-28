#pragma once
// Qt basic libraries
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPixmap>
#include <QPushButton>
#include <QString>
#include <QtNetwork/QHostAddress>
#include <QVBoxLayout>
#include <QWidget>
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
	QString local_ip;
	QString mac_address;
	QHostAddress address;

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

	inline void setResolved(const QHostAddress& address_) {
		is_resolved = true;
		address = address_;
		address_label = new QLabel(address.toString());
		address_label->setTextInteractionFlags(Qt::TextSelectableByMouse);
		name_layout->addWidget(address_label);
		connection_status_icon->setStyleSheet("QLabel {background-color : yellow}");
	}

	inline QString getAddress() {
		return address.toString();
	}

	inline bool getResolved() {
		return is_resolved;
	}

	inline void setAlreadyAdded() {
		is_added = true;
		connection_status_icon->setStyleSheet("QLabel {background-color : blue}");
	}

	inline bool getAdded() {
		return is_added;
	}

	inline void setLocalIP(const QString& local_ip_) {
		local_ip = local_ip_;
		connection_status_icon->setStyleSheet("QLabel {background-color : green}");
	}

	inline QString getLocalIP() {
		return local_ip;
	}

	inline void setMac(const QString& mac_address_) {
		mac_address = mac_address_;
	}

	inline QString getMac() {
		return mac_address;
	}
};
