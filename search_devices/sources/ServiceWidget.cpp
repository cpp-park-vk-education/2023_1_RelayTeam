#include "ServiceWidget.h"
#include "variables.h"

ServiceItem::ServiceItem(const QMdnsEngine::Service& service_) : service(service_), local_ip("") {
	main_widget = new QWidget();
	main_layout = new QHBoxLayout();  // Creating layout.
	main_widget->setLayout(main_layout);
	main_layout->addStretch();
	main_layout->setSizeConstraint(QLayout::SetFixedSize);
	connection_status_icon = new QLabel();
	connection_status_icon->setStyleSheet("QLabel{background-color: red; color: black;}");
	connection_status_icon->setFixedSize(15, 50);
	main_layout->addWidget(connection_status_icon);
	QPixmap icon_pixmap;  // Creating up OS icon.
	QStringList os_types;
	os_types << "linux"	   // 0
			 << "windows"  // 1
			 << "android"  // 2
			 << "darwin";  // 3
	switch (os_types.indexOf(service.attributes()["OS_type"])) {
		case 0:
			icon_pixmap.load(Q_RESOURCE_DIR.absoluteFilePath("linux.png"));
			break;
		case 1:
			icon_pixmap.load(Q_RESOURCE_DIR.absoluteFilePath("windows.png"));
			break;
		case 2:
			icon_pixmap.load(Q_RESOURCE_DIR.absoluteFilePath("android.png"));
			break;
		case 3:
			icon_pixmap.load(Q_RESOURCE_DIR.absoluteFilePath("apple.png"));
			break;
		default:
			icon_pixmap.load(Q_RESOURCE_DIR.absoluteFilePath("unknown_os.png"));
	}
	os_icon_label = new QLabel();
	os_icon_label->setFixedSize(50, 50);
	os_icon_label->setPixmap(icon_pixmap.scaled(50, 50));
	main_layout->addWidget(static_cast<QWidget*>(os_icon_label));
	service_name_label = new QLabel(service.name());  // creating name label
	service_name_label->setFont(QFont("Arial", 16));
	main_layout->addWidget(static_cast<QWidget*>(service_name_label));
	this->setSizeHint(main_widget->sizeHint());	 // setting item to fit content
}

void ServiceItem::update(const QMdnsEngine::Service& service_) {
	service = service_;
	service_name_label->setText(service.name());
}
