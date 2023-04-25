#include "MainWindow.h"
#include <iostream>
#include <QMessageBox>
#include <QSlider>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), current_search_widget_is_manual(true), current_control_widget_is_settings(false) {
	current_options = new Options();
	data_base.getOptions(current_options);
	main_widget = new QWidget();  // Setting up main window widgets.
	main_layout = new QHBoxLayout();
	main_widget->setLayout(main_layout);
	main_widget->setMinimumSize(800, 300);
	this->setCentralWidget(main_widget);
	left_bar = new QVBoxLayout;
	main_layout->addLayout(left_bar);
	button_layout = new QHBoxLayout();					  // Creating button layout.
	scan_network_button = new QPushButton("Scan local");  // Creating settings button.
	scan_network_button->setFixedHeight(60);
	connect(scan_network_button, &QPushButton::clicked, this, &MainWindow::onScanNetworkButtonPressed);
	button_layout->addWidget(scan_network_button);
	settings_button = new QPushButton("Settings");	// Creating settings button.
	settings_button->setFixedHeight(60);
	connect(settings_button, &QPushButton::clicked, this, &MainWindow::onSettingsButtonPressed);
	button_layout->addWidget(settings_button);
	left_bar->addLayout(button_layout);
	add_button = new QPushButton("Add");
	add_button->setFixedHeight(60);
	left_bar->addWidget(add_button);
	input_box = new QTextEdit("Text box");	// Creating input box.
	left_bar->addWidget(input_box);
	devices_layout = new QVBoxLayout();	 // Creating devices layout with scroll area.
	data_base.getDevices(devices_layout);
	devices_layout->setContentsMargins(0, 0, 0, 0);
	devices_widget = new QWidget();
	devices_widget->sizeHint();
	devices_widget->setLayout(devices_layout);
	devices_scroll_area = new QScrollArea();
	devices_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	devices_scroll_area->setWidgetResizable(true);
	devices_scroll_area->setAlignment(Qt::AlignRight);
	devices_scroll_area->setMinimumWidth(500);
	devices_scroll_area->setMaximumWidth(620);
	devices_scroll_area->setWidget(devices_widget);
	main_layout->addWidget(devices_scroll_area);

	search_widget = new SearchWidget();	 // Creating network widgets.
	search_widget->hide();
	left_bar->addWidget(search_widget);
	connect(add_button, &QPushButton::clicked, search_widget, &SearchWidget::onAddButtonCLicked);
	connect(search_widget, &SearchWidget::devicePreparedToAdd, this, &MainWindow::onDevicePreparedToAdd);
	publisher_widget = new Publisher(current_options->device_name, this);

	settings_widget = new SettingsWidget(current_options);
	settings_widget->setFixedWidth(500);
	main_layout->addWidget(settings_widget);
	settings_widget->hide();
	connect(settings_widget, &SettingsWidget::sendChangeServiceName, publisher_widget, &Publisher::onChangeServiceName);

	main_widget->show();
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

void MainWindow::saveAllChanges() {
	for (size_t i = 0; i < devices_layout->count(); ++i) {
		data_base.saveDeviceChanges(static_cast<DeviceWidget*>(devices_layout->itemAt(i)));
	}
	data_base.saveOptionsChanges(current_options);
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

void MainWindow::onDevicePreparedToAdd(QString name, QString local_ip) {
	qDebug() << "Adding device: " << name;
	DeviceWidget* device_widget = new DeviceWidget(-1, name, 0);
	devices_layout->addLayout(device_widget);
}
