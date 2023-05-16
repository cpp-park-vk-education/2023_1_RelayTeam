#include "MainWindow.h"

#include <UITools.h>

#include <QMessageBox>
#include <QSlider>
#include <iostream>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), current_search_widget_is_manual(true), current_control_widget_is_settings(false), streaming_session_manager() {
	options = new Options();
	data_base.getOptions(options);
	QFont font = this->font();
	font.setPointSize(16 * getFontScaling(options->getScale()));
	this->setFont(font);
	main_widget = new QWidget();  // Setting up main window widgets.
	main_layout = new QHBoxLayout();
	main_widget->setLayout(main_layout);
	main_widget->setMinimumSize(920 * options->getScale(), 400 * options->getScale());
	this->setCentralWidget(main_widget);
	right_bar = new QVBoxLayout();                  // Creating right bar.
	settings_button = new QPushButton("Settings");  // Creating settings button.
	settings_button->setFixedHeight(60 * options->getScale());
	connect(settings_button, &QPushButton::clicked, this, &MainWindow::onSettingsButtonPressed);
	right_bar->addWidget(settings_button);
	devices_layout = new QVBoxLayout();  // Creating devices layout with scroll area.
	data_base.getDevices(devices_layout, options->getScale());
	for (size_t i = 0; i < devices_layout->count(); ++i) {
		makeDeviceConnections(static_cast<DeviceWidget*>(devices_layout->itemAt(i)));
	}
	devices_layout->setContentsMargins(0, 0, 0, 0);
	devices_widget = new QWidget();  // Creating widget for devices layout
	devices_widget->sizeHint();
	devices_widget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Maximum);
	devices_widget->setLayout(devices_layout);
	devices_scroll_area = new QScrollArea();  // Creating and configuring scroll area for devices widget.
	devices_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	devices_scroll_area->setWidgetResizable(true);
	devices_scroll_area->setAlignment(Qt::AlignRight);
	devices_scroll_area->setMinimumWidth(570 * options->getScale());
	devices_scroll_area->setMaximumWidth(700 * options->getScale());
	devices_scroll_area->setWidget(devices_widget);
	right_bar->addWidget(devices_scroll_area);
	settings_widget = new SettingsWidget(options);  // Creating settings widget.
	settings_widget->setFixedWidth(570 * options->getScale());
	settings_widget->hide();
	right_bar->addWidget(settings_widget);
	left_bar = new QVBoxLayout();  // creating left bar
	left_bar->setAlignment(Qt::AlignLeft);
	button_layout = new QHBoxLayout();                    // Creating button layout.
	scan_network_button = new QPushButton("Scan local");  // Creating scan local button.
	scan_network_button->setFixedHeight(60 * options->getScale());
	connect(scan_network_button, &QPushButton::clicked, this, &MainWindow::onScanNetworkButtonPressed);
	button_layout->addWidget(scan_network_button);
	add_button = new QPushButton("Add");  // Creatint add button
	add_button->setFixedHeight(60 * options->getScale());
	button_layout->addWidget(add_button);
	left_bar->addLayout(button_layout);
	input_box = new QTextEdit("Text box");  // Creating input box.
	left_bar->addWidget(input_box);
	search_widget = new SearchWidget(options->getScale());  // Creating network widgets.
	search_widget->hide();
	left_bar->addWidget(search_widget);
	connect(add_button, &QPushButton::clicked, search_widget, &SearchWidget::onAddButtonCLicked);
	connect(&streaming_session_manager, &SessionManager::sendStartReciver, search_widget, &SearchWidget::onStartReciver);
	connect(search_widget, &SearchWidget::devicePreparedToAdd, this, &MainWindow::onDevicePreparedToAdd);
	connect(search_widget, &SearchWidget::sendUpdateAddress, this, &MainWindow::onUpdateAddress);
	publisher_widget = new Publisher(options->device_name, this);
	connect(this, &MainWindow::sendDeviceIdsUpdated, search_widget, &SearchWidget::onDeviceIdsUpdated);
	getDeviceIds();
	connect(settings_widget, &SettingsWidget::sendChangeServiceName, publisher_widget, &Publisher::onChangeServiceName);
	connect(publisher_widget, &Publisher::sendStartReceivingSession, &streaming_session_manager, &SessionManager::onStartReceivingSession);
	connect(&streaming_session_manager, &SessionManager::sendSetPorts, publisher_widget, &Publisher::onSendPorts);
	connect(search_widget, &SearchWidget::sendReceivedPorts, &streaming_session_manager, &SessionManager::onReceivedPorts);

	main_layout->addLayout(left_bar);
	main_layout->addLayout(right_bar);
	main_widget->show();
}

MainWindow::~MainWindow() {
	delete options;
}

void MainWindow::makeDeviceConnections(DeviceWidget* device) {
	connect(device, &DeviceWidget::sendStartVideoSession, &streaming_session_manager, &SessionManager::onStartVideoSession);
	connect(device, &DeviceWidget::sendStopVideoSession, &streaming_session_manager, &SessionManager::onKillVideoSession);
	connect(device, &DeviceWidget::sendStartAudioSession, &streaming_session_manager, &SessionManager::onStartAudioSession);
	connect(device, &DeviceWidget::sendStopAudioSession, &streaming_session_manager, &SessionManager::onKillAudioSession);
}

void MainWindow::onSettingsButtonPressed() {
	if (current_control_widget_is_settings) {
		settings_widget->hide();
		devices_scroll_area->show();
		settings_button->setText("Settings");
	} else {
		settings_widget->show();
		devices_scroll_area->hide();
		settings_button->setText("View Devices");
	}
	current_control_widget_is_settings = !current_control_widget_is_settings;
}

void MainWindow::onScanNetworkButtonPressed() {
	if (current_search_widget_is_manual) {
		input_box->hide();
		search_widget->show();
		scan_network_button->setText("Manual pairing");
	} else {
		search_widget->hide();
		input_box->show();
		scan_network_button->setText("Scan local");
	}
	current_search_widget_is_manual = !current_search_widget_is_manual;
}
void MainWindow::saveAllChanges()

{
	for (size_t i = 0; i < devices_layout->count(); ++i) {
		data_base.saveDeviceChanges(static_cast<DeviceWidget*>(devices_layout->itemAt(i)));
	}
	data_base.saveOptionsChanges(options);
}

void MainWindow::closeEvent(QCloseEvent* event) {
	saveAllChanges();
	QMessageBox::StandardButton resBtn =
		QMessageBox::question(this, "Warning", tr("Exit the application?\n"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
	if (resBtn != QMessageBox::Yes) {
		event->ignore();
	} else {
		event->accept();
	}
}

void MainWindow::onDevicePreparedToAdd(QString name, QHostAddress local_ipv4_address, QString mac_address) {
	qDebug() << "Adding device: " << name;
	DeviceWidget* device_widget = new DeviceWidget(mac_address, name, local_ipv4_address, 50, options->getScale());
	makeDeviceConnections(device_widget);
	devices_layout->addLayout(device_widget);
	data_base.addDevice(device_widget);
	device_ids.insert(mac_address);
	emit sendDeviceIdsUpdated(device_ids);
}

void MainWindow::getDeviceIds() {
	for (size_t i = 0; i < devices_layout->count(); ++i) {
		device_ids.insert(static_cast<DeviceWidget*>(devices_layout->itemAt(i))->ID);
	}
	emit sendDeviceIdsUpdated(device_ids);
}

void MainWindow::onUpdateAddress(QString mac_address, QHostAddress local_ipv4_address) {
	for (size_t i = 0; i < devices_layout->count(); ++i) {
		if (static_cast<DeviceWidget*>(devices_layout->itemAt(i))->ID == mac_address) {
			qDebug() << "Updating local ipv4 for" << static_cast<DeviceWidget*>(devices_layout->itemAt(i))->name;
			static_cast<DeviceWidget*>(devices_layout->itemAt(i))->local_ipv4_address = local_ipv4_address;
			return;
		}
	}
}
