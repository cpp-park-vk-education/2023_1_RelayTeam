#include "Device.h"

DeviceWidget::DeviceWidget(QString name_, QWidget *parent) : QHBoxLayout(parent) {
	QLabel *name_box = new QLabel(name_);  // Creating label with device name.
	this->addWidget(name_box);
	QSlider *volume_slider = new QSlider(Qt::Horizontal);  // Creating volume slider.
	volume_slider->setFixedWidth(160);
	volume_slider->setMaximum(100);
	this->addWidget(volume_slider);
	QSpinBox *volume_box = new QSpinBox();	// Creating volume level label.
	volume_box->setFixedSize(50, 50);
	volume_box->setMaximum(100);
	this->addWidget(volume_box);
	connect(volume_slider, SIGNAL(valueChanged(int)), volume_box, SLOT(setValue(int)));	 // Connecting slider and label
	connect(volume_box, SIGNAL(valueChanged(int)), volume_slider, SLOT(setValue(int)));
	audio_button = new QPushButton();  // Creating audio toggle button.
	audio_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("audio-disabled.png")));
	audio_button->setIconSize(QSize(40, 40));
	audio_button->setFixedSize(50, 50);
	connect(audio_button, SIGNAL(clicked()), this, SLOT(onAudioPressed()));
	this->addWidget(audio_button);
	cast_button = new QPushButton();  // Creating screen cast toggle button.
	cast_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("cast-disabled.png")));
	cast_button->setIconSize(QSize(40, 40));
	cast_button->setFixedSize(50, 50);
	connect(cast_button, SIGNAL(clicked()), this, SLOT(onCastPressed()));
	this->addWidget(cast_button);
	settings_button = new QPushButton();  // Creating settings toggle button.
	settings_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("settings.png")));
	settings_button->setIconSize(QSize(40, 40));
	settings_button->setFixedSize(50, 50);
	connect(settings_button, SIGNAL(clicked()), this, SLOT(onSettingsPressed()));
	this->addWidget(settings_button);
}

void DeviceWidget::onAudioPressed() {
	if (info.audio_state) {
		audio_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("audio-disabled.png")));
	} else {
		audio_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("audio.png")));
	}
	info.audio_state = !info.audio_state;
}

void DeviceWidget::onCastPressed() {
	if (info.cast_state) {
		cast_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("cast-disabled.png")));
	} else {
		cast_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("cast.png")));
	}
	info.cast_state = !info.cast_state;
}

void DeviceWidget::onSettingsPressed() {}
