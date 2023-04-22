#include "MainWindow.h"
#include <iostream>
#include <QMessageBox>
#include <QSlider>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	main_widget = new QWidget();  // Setting up main window widgets
	main_grid = new QGridLayout();
	main_widget->setLayout(main_grid);
	main_widget->setMinimumSize(750, 300);
	this->setCentralWidget(main_widget);
	left_bar = new QVBoxLayout;
	main_grid->addLayout(left_bar, 1, 0);
	settings_button = new QPushButton("Settings");	// Creating settings button.
	settings_button->setFixedHeight(60);
	connect(settings_button, &QPushButton::clicked, this, &MainWindow::onSettingsButtonPressed);
	left_bar->addWidget(settings_button);
	scan_network_button = new QPushButton("Scan local network");  // Creating settings button.
	scan_network_button->setFixedHeight(60);
	connect(scan_network_button, &QPushButton::clicked, this, &MainWindow::onScanNetworkButtonPressed);
	left_bar->addWidget(scan_network_button);
	input_box = new QTextEdit("Text box");	// Creating input box.
	left_bar->addWidget(input_box);
	devices_layout = new QVBoxLayout();	 // Creating devices layout with scroll area.
	data_base.getDevices(devices_layout);
	devices_layout->setContentsMargins(0, 0, 0, 0);
	QFrame* devices_layout_container = new QFrame();
	devices_layout_container->setLayout(devices_layout);
	devices_scroll_area = new QScrollArea();
	devices_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	devices_scroll_area->setAlignment(Qt::AlignRight);
	devices_scroll_area->setFixedWidth(500);
	devices_scroll_area->setWidget(devices_layout_container);
	main_grid->addWidget(devices_scroll_area, 1, 1);

	search_w = new SearchWidget();	// launching network widgets
	left_bar->addWidget(search_w);
	publisher_widget = new Publisher(this);

	search_w->hide();
	main_widget->show();
}

void MainWindow::onSettingsButtonPressed() {}

void MainWindow::onScanNetworkButtonPressed() {
	if (layout_state) {
		search_w->hide();
		input_box->show();
		scan_network_button->setText("Scan local network");
	} else {
		input_box->hide();
		search_w->show();
		scan_network_button->setText("Manual pairing");
	}
	layout_state = !layout_state;
}

void MainWindow::closeEvent(QCloseEvent* event) {
	for (size_t i = 0; i < devices_layout->count(); ++i) {
		data_base.saveDeviceChanges(static_cast<DeviceWidget*>(devices_layout->itemAt(i)));
	}
	QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Warning", tr("Are you sure what to close the application?\n"),
															   QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
	if (resBtn != QMessageBox::Yes) {
		event->ignore();
	} else {
		event->accept();
	}
	search_w->hide();
}
