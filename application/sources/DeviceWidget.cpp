#include "DeviceWidget.h"

#include <QDebug>
#include <QtMath>

void DeviceWidget::defineWdgets(qreal scale) {
	this->setMargin(0);
	this->setAlignment(Qt::AlignTop);
	name_box = new QLabel(name);  // Creating label with device name.
	name_box->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	name_box->setFixedHeight(50 * scale);
	this->addWidget(name_box);
	volume_slider = new QSlider(Qt::Horizontal);  // Creating volume slider.
	volume_slider->setFixedWidth(160 * scale);
	volume_slider->setMaximum(100);
	this->addWidget(volume_slider);
	volume_box = new QSpinBox();  // Creating volume level label.
	volume_box->setFixedSize(50 * (scale < 1 ? qSqrt(scale) : scale), 50 * scale);
	volume_box->setMaximum(100);
	this->addWidget(volume_box);
	connect(volume_slider, &QSlider::valueChanged, volume_box, &QSpinBox::setValue);  // Connecting slider and label
	connect(volume_slider, &QSlider::valueChanged, this, &DeviceWidget::onVolumeChanged);
	connect(volume_box, qOverload<int>(&QSpinBox::valueChanged), volume_slider, &QSlider::setValue);
	volume_slider->setValue(volume);
	audio_button = new QPushButton();  // Creating audio toggle button.
	audio_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("audio-disabled.png")));
	audio_button->setIconSize(QSize(40 * scale, 40 * scale));
	audio_button->setFixedSize(50 * scale, 50 * scale);
	connect(audio_button, &QPushButton::clicked, this, &DeviceWidget::onAudioPressed);
	this->addWidget(audio_button);
	cast_button = new QPushButton();  // Creating screen cast toggle button.
	cast_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("cast-disabled.png")));
	cast_button->setIconSize(QSize(40 * scale, 40 * scale));
	cast_button->setFixedSize(50 * scale, 50 * scale);
	connect(cast_button, &QPushButton::clicked, this, &DeviceWidget::onCastPressed);
	this->addWidget(cast_button);
	settings_button = new QPushButton();  // Creating settings toggle button.
	settings_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("settings.png")));
	settings_button->setIconSize(QSize(40 * scale, 40 * scale));
	settings_button->setFixedSize(50 * scale, 50 * scale);
	connect(settings_button, &QPushButton::clicked, this, &DeviceWidget::onSettingsPressed);
	this->addWidget(settings_button);
}

DeviceWidget::DeviceWidget(QString ID_, const QString& name_, const QHostAddress& local_ipv4_address_, qint16 volume_, qreal scale,
						   QWidget* parent)
	: ID(ID_), name(name_), volume(volume_), local_ipv4_address(local_ipv4_address_), QHBoxLayout(parent) {
	defineWdgets(scale);
}

DeviceWidget::DeviceWidget(QString ID_, QString&& name_, const QHostAddress& local_ipv4_address_, qint16 volume_, qreal scale,
						   QWidget* parent)
	: ID(ID_), name(std::move(name_)), volume(volume_), local_ipv4_address(local_ipv4_address_), QHBoxLayout(parent) {
	defineWdgets(scale);
}

void DeviceWidget::onAudioPressed() {
	if (audio_state) {
		audio_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("audio-disabled.png")));
		emit sendStopAudioSession(local_ipv4_address);
	} else {
		audio_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("audio.png")));
		emit sendStartAudioSession(local_ipv4_address);
	}
	audio_state = !audio_state;
}

void DeviceWidget::onCastPressed() {
	if (cast_state) {
		cast_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("cast-disabled.png")));
		emit sendStopVideoSession(local_ipv4_address);
	} else {
		cast_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("cast.png")));
		emit sendStartVideoSession(local_ipv4_address);
	}
	cast_state = !cast_state;
}

void DeviceWidget::onSettingsPressed() {
	// DeviceSettingsDialog settings_widget = new DeviceSettingsDialog;
}

void DeviceWidget::onVolumeChanged(qint16 volume_) {
	volume = volume_;
}
