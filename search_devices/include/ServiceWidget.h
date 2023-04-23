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

class ServiceWidget : public QWidget {
private:
	Q_OBJECT
	QLabel* service_name_label;
	QLabel* machine_id_label;
	QHBoxLayout* main_layout;
	QListWidgetItem* item;

public:
	ServiceWidget(const QMdnsEngine::Service& service, QListWidgetItem* item, QWidget* parent = nullptr);

	inline QListWidgetItem* get_item() {
		return item;
	}
};
