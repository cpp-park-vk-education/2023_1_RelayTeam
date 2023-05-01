#include "SettingsWidget.h"

#include <UITools.h>

#include <QtMath>

SettingsWidget::SettingsWidget(Options* options_, QWidget* parrent) : QWidget(parrent), options(options_) {
	QFont font = this->font();
	font.setPointSize(14 * getFontScaling(options->getScale()));
	this->setFont(font);
	qreal scale = options->getScale();
	main_layout = new QVBoxLayout();
	this->setLayout(main_layout);
	change_device_name_layout = new QHBoxLayout();
	main_layout->addLayout(change_device_name_layout);
	reset_device_name_button = new QPushButton("Reset device name");
	reset_device_name_button->setFixedSize(170 * scale, 40 * scale);
	change_device_name_layout->addWidget(reset_device_name_button);
	set_device_name_button = new QPushButton("Set device name");
	change_device_name_layout->addWidget(set_device_name_button);
	set_device_name_button->setFixedSize(170 * scale, 40 * scale);
	device_name_input_box = new QLineEdit();
	device_name_input_box->setFixedHeight(40 * scale);
	device_name_input_box->setPlaceholderText(options->device_name);
	device_name_input_box->setMaxLength(20);
	change_device_name_layout->addWidget(device_name_input_box);
	connect(set_device_name_button, &QPushButton::clicked, this, &SettingsWidget::onSetDeviceNameButtonClicked);
	connect(reset_device_name_button, &QPushButton::clicked, this, &SettingsWidget::onResetDeviceNameButtonClicked);
	change_scale_layout = new QHBoxLayout();
	main_layout->addLayout(change_scale_layout);
	scaling_label = new QLabel("UI scaling: ");
	change_scale_layout->addWidget(scaling_label);
	scale_slider = new QSlider(Qt::Horizontal);  // Creating scale slider.
	scale_slider->setFixedWidth(160 * scale);
	scale_slider->setMaximum(200);
	scale_slider->setMinimum(50);
	change_scale_layout->addWidget(scale_slider);
	scale_box = new QSpinBox();  // Creating scale label.
	scale_box->setFixedSize(70 * scale, 50 * scale);
	scale_box->setMaximum(200);
	scale_box->setMinimum(50);
	change_scale_layout->addWidget(scale_box);
	scaling_warning_label = new QLabel("Requires a restart.");
	scaling_warning_label->setStyleSheet("QLabel { color : grey; }");
	scaling_warning_label->setFont(font);
	change_scale_layout->addWidget(scaling_warning_label);
	connect(scale_slider, &QSlider::valueChanged, scale_box,
			&QSpinBox::setValue);  // Connecting slider and label
	connect(scale_slider, &QSlider::valueChanged, this, &SettingsWidget::onScaleChanged);
	connect(scale_box, qOverload<int>(&QSpinBox::valueChanged), scale_slider, &QSlider::setValue);
	scale_box->setValue(options->scale_factor);
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

void SettingsWidget::onScaleChanged(int scale_) {
	if (scale_ != options->scale_factor) {
		scaling_warning_label->setStyleSheet("QLabel { color : red; }");
	} else {
		scaling_warning_label->setStyleSheet("QLabel { color : grey; }");
	}
	options->scale_factor_new = scale_;
}
