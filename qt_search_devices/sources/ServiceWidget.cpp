#include "ServiceWidget.h"
#include "variables.h"

ServiceWidget::ServiceWidget(const QMdnsEngine::Service& service, QListWidgetItem* item, QWidget* parent) : QWidget(parent) {
	main_layout = new QHBoxLayout;	// creating layout
	this->setLayout(main_layout);
	main_layout->addStretch();
	main_layout->setSizeConstraint(QLayout::SetFixedSize);
	QPixmap* icon_pixmap = new QPixmap();
	QStringList os_types;
	os_types << "linux"	   // 0
			 << "windows"  // 1
			 << "android"  // 2
			 << "apple";   // 3
	switch (os_types.indexOf(service.attributes()["OS_type"])) {
		case 0:
			icon_pixmap->load(Q_RESOURCE_DIR.absoluteFilePath("linux.png"));
			break;
		case 1:
			icon_pixmap->load(Q_RESOURCE_DIR.absoluteFilePath("windows.png"));
			break;
		case 2:
			icon_pixmap->load(Q_RESOURCE_DIR.absoluteFilePath("android.png"));
			break;
		case 3:
			icon_pixmap->load(Q_RESOURCE_DIR.absoluteFilePath("apple.png"));
			break;
		default:
			icon_pixmap->load(Q_RESOURCE_DIR.absoluteFilePath("unknown_os.png"));
	}
	QLabel* os_icon_label = new QLabel();
	os_icon_label->setFixedSize(50, 50);

	os_icon_label->setPixmap(icon_pixmap->scaled(50, 50));
	main_layout->addWidget(static_cast<QWidget*>(os_icon_label));
	service_name_label = new QLabel(service.name());  // creating name label
	service_name_label->setFont(QFont("Arial", 16));
	main_layout->addWidget(static_cast<QWidget*>(service_name_label));
	//	machine_id_label = new QLabel(service.attributes()["mID"]);
	//	machine_id_label->setFont(QFont("Arial", 10));
	//	main_layout->addWidget(static_cast<QWidget*>(machine_id_label));
	item->setSizeHint(this->sizeHint());  // setting item to fit content
}
