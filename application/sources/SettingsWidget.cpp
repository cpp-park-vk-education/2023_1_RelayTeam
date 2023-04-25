#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(Options* options_, QWidget* parrent) : QWidget(parrent), options(options_) {
	main_layout = new QVBoxLayout();
	this->setLayout(main_layout);
	change_device_name_layout = new QHBoxLayout();
	main_layout->addLayout(change_device_name_layout);
	reset_device_name_button = new QPushButton("Reset device name");
	reset_device_name_button->setFixedSize(135, 40);
	change_device_name_layout->addWidget(reset_device_name_button);
	set_device_name_button = new QPushButton("Set device name");
	change_device_name_layout->addWidget(set_device_name_button);
	set_device_name_button->setFixedSize(130, 40);
	device_name_input_box = new QLineEdit();
	device_name_input_box->setFixedSize(200, 40);
	device_name_input_box->setPlaceholderText(options->device_name);
	device_name_input_box->setMaxLength(20);
	change_device_name_layout->addWidget(device_name_input_box);
	connect(set_device_name_button, &QPushButton::clicked, this, &SettingsWidget::onSetDeviceNameButtonClicked);
	connect(reset_device_name_button, &QPushButton::clicked, this, &SettingsWidget::onResetDeviceNameButtonClicked);
}

void SettingsWidget::onSetDeviceNameButtonClicked() {
	if (device_name_input_box->text().size() == 0) {
		return;
	}
	options->device_name = device_name_input_box->text();
	device_name_input_box->setPlaceholderText(device_name_input_box->text());
	emit sendChangeServiceName(options->device_name);
}

void SettingsWidget::onResetDeviceNameButtonClicked() {
	options->device_name = QSysInfo::machineHostName();
	device_name_input_box->setPlaceholderText(QSysInfo::machineHostName());
	emit sendChangeServiceName(QSysInfo::machineHostName());
}
