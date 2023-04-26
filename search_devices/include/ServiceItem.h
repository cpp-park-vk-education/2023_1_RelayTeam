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
// qmdnsengine
#include <qmdnsengine/service.h>

class ServiceItem : public QListWidgetItem {
private:
	QLabel* service_name_label;
	QLabel* connection_status_icon;
	QLabel* os_icon_label;
	QHBoxLayout* main_layout;
	QPushButton* add_button;
	QWidget* main_widget;
	QMdnsEngine::Service service;

	bool already_added;
	QString local_ip;

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

	inline void setResolved() {
		connection_status_icon->setStyleSheet("QLabel {background-color : yellow}");
	}

	inline void setAlreadyAdded() {
		already_added = true;
		main_widget->setStyleSheet("QWidget { background-color : grey; }");
	}

	inline bool getAdded() {
		return already_added;
	}

	inline void setLocalIP(const QString& local_ip_) {
		local_ip = local_ip_;
		connection_status_icon->setStyleSheet("QLabel {background-color : green}");
	}

	inline QString getLocalIP() {
		return local_ip;
	}
};
