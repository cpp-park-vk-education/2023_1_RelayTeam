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
	QLabel* machine_id_label;
	QHBoxLayout* main_layout;
	QLabel* os_icon_label;
	QPushButton* add_button;
	QWidget* main_widget;
	QMdnsEngine::Service service;

	QString local_ip;

public:
	ServiceItem(const QMdnsEngine::Service& service_);

	~ServiceItem() {}

	void update(const QMdnsEngine::Service& service_);

	inline QWidget* getWidget() {
		return main_widget;
	}

	inline QMdnsEngine::Service getService() {
		return service;
	}

	inline void setResolved() {
		add_button->setStyleSheet("QPushButton {background-color : yellow}");
	}

	inline void setLocalIP(const QString& local_ip_) {
		local_ip = local_ip_;
		add_button->setStyleSheet("QPushButton {background-color : green}");
		add_button->setEnabled(true);
	}
};
