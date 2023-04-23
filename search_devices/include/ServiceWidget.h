#pragma once
// Qt basic libraries
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPixmap>
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
	QLabel* resolved_icon;
	QWidget* main_widget;
	QMdnsEngine::Service service;

public:
	ServiceItem(const QMdnsEngine::Service& service_);

	void update(const QMdnsEngine::Service& service_);

	inline QWidget* getWidget() {
		return main_widget;
	}

	inline QMdnsEngine::Service getService() {
		return service;
	}

	inline void setUnResolved() {
		resolved_icon->setStyleSheet("QLabel {background-color : red}");
	}

	inline void setResolved() {
		resolved_icon->setStyleSheet("QLabel {background-color : yellow}");
	}

	inline void setGotLocalIP() {
		resolved_icon->setStyleSheet("QLabel {background-color : green}");
	}
};
