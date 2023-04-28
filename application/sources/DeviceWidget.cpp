#include "DeviceWidget.h"
#include <QDebug>

void DeviceWidget::defineWdgets() {
	QLabel* name_box = new QLabel(name);  // Creating label with device name.
	this->addWidget(name_box);
	QSlider* volume_slider = new QSlider(Qt::Horizontal);  // Creating volume slider.
	volume_slider->setFixedWidth(160);
	volume_slider->setMaximum(100);
	this->addWidget(volume_slider);
	QSpinBox* volume_box = new QSpinBox();	// Creating volume level label.
	volume_box->setFixedSize(50, 50);
	volume_box->setMaximum(100);
	this->addWidget(volume_box);
	connect(volume_slider, &QSlider::valueChanged, volume_box, &QSpinBox::setValue);  // Connecting slider and label
	connect(volume_slider, &QSlider::valueChanged, this, &DeviceWidget::onVolumeChanged);
	connect(volume_box, qOverload<int>(&QSpinBox::valueChanged), volume_slider, &QSlider::setValue);
	volume_slider->setValue(volume);
	audio_button = new QPushButton();  // Creating audio toggle button.
	audio_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("audio-disabled.png")));
	audio_button->setIconSize(QSize(40, 40));
	audio_button->setFixedSize(50, 50);
	connect(audio_button, &QPushButton::clicked, this, &DeviceWidget::onAudioPressed);
	this->addWidget(audio_button);
	cast_button = new QPushButton();  // Creating screen cast toggle button.
	cast_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("cast-disabled.png")));
	cast_button->setIconSize(QSize(40, 40));
	cast_button->setFixedSize(50, 50);
	connect(cast_button, &QPushButton::clicked, this, &DeviceWidget::onCastPressed);
	this->addWidget(cast_button);
	settings_button = new QPushButton();  // Creating settings toggle button.
	settings_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("settings.png")));
	settings_button->setIconSize(QSize(40, 40));
	settings_button->setFixedSize(50, 50);
	connect(settings_button, &QPushButton::clicked, this, &DeviceWidget::onSettingsPressed);
	this->addWidget(settings_button);
}

DeviceWidget::DeviceWidget(int ID_, QString& name_, int volume_, QWidget* parent)
	: ID(ID_), name(name_), volume(volume_), QHBoxLayout(parent) {
	defineWdgets();
}

DeviceWidget::DeviceWidget(int ID_, QString&& name_, int volume_, QWidget* parent)
	: ID(ID_), name(std::move(name_)), volume(volume_), QHBoxLayout(parent) {
	defineWdgets();
}

void DeviceWidget::onAudioPressed() {
	if (audio_state) {
		audio_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("audio-disabled.png")));
	} else {
		audio_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("audio.png")));
	}
	audio_state = !audio_state;
}

void DeviceWidget::onCastPressed()
{
    if (cast_state) {
        cast_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("cast-disabled.png")));
    } else {
        cast_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("cast.png")));
    }
    cast_state = !cast_state;
}

void DeviceWidget::onSettingsPressed() {
	// DeviceSettingsDialog settings_widget = new DeviceSettingsDialog;
}

void DeviceWidget::onVolumeChanged(int volume_)
{
    volume = volume_;
}

//fdsfsdf
