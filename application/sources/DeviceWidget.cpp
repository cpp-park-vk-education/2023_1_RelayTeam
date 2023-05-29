#include "DeviceWidget.h"

#include <QDebug>
#include <QtMath>

void DeviceWidget::setBasicControlLayout() {
    name_box = new QLabel(name);  // Creating label with device name.
    name_box->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    name_box->setFixedHeight(50 * scale);
    basic_control_layout->addWidget(name_box);
    volume_slider = new QSlider(Qt::Horizontal);  // Creating volume slider.
    volume_slider->setFixedWidth(140 * scale);
    volume_slider->setMaximum(100);
    basic_control_layout->addWidget(volume_slider);
    volume_box = new QSpinBox();  // Creating volume level label.
    volume_box->setFixedSize(60 * (scale < 1 ? qSqrt(scale) : scale), 50 * scale);
    volume_box->setMaximum(100);
    basic_control_layout->addWidget(volume_box);
    connect(volume_slider, &QSlider::valueChanged, volume_box, &QSpinBox::setValue);  // Connecting slider and label
    connect(volume_slider, &QSlider::valueChanged, this, &DeviceWidget::onVolumeChanged);
    connect(volume_box, qOverload<int>(&QSpinBox::valueChanged), volume_slider, &QSlider::setValue);
    volume_slider->setValue(volume);
    audio_button = new QPushButton();  // Creating audio toggle button.
    audio_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("audio-disabled.png")));
    audio_button->setIconSize(QSize(40 * scale, 40 * scale));
    audio_button->setFixedSize(50 * scale, 50 * scale);
    connect(audio_button, &QPushButton::clicked, this, &DeviceWidget::onAudioPressed);
    basic_control_layout->addWidget(audio_button);
    cast_button = new QPushButton();  // Creating screen cast toggle button.
    cast_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("cast-disabled.png")));
    cast_button->setIconSize(QSize(40 * scale, 40 * scale));
    cast_button->setFixedSize(50 * scale, 50 * scale);
    connect(cast_button, &QPushButton::clicked, this, &DeviceWidget::onCastPressed);
    basic_control_layout->addWidget(cast_button);
    settings_button = new QPushButton();  // Creating settings toggle button.
    settings_button->setIcon(QIcon(Q_RESOURCE_DIR.absoluteFilePath("settings.png")));
    settings_button->setIconSize(QSize(40 * scale, 40 * scale));
    settings_button->setFixedSize(50 * scale, 50 * scale);
    connect(settings_button, &QPushButton::clicked, this, &DeviceWidget::onSettingsPressed);
    basic_control_layout->addWidget(settings_button);
}

void DeviceWidget::setAdditionalControlLayout() {
    QRadioButton* screen_capture_button = new QRadioButton;
    screen_capture_button->setText("Screen capture");
    screen_capture_button->setChecked(source_mode_is_screen);
    connect(screen_capture_button, &QPushButton::clicked, this, &DeviceWidget::onSetScreenCaptureMode);
    additional_control_layout->addWidget(screen_capture_button);
    QRadioButton* camera_capture_button = new QRadioButton;
    camera_capture_button->setText("Camera capture");
    camera_capture_button->setChecked(!source_mode_is_screen);
    connect(camera_capture_button, &QPushButton::clicked, this, &DeviceWidget::onSetCameraCaptureMode);
    additional_control_layout->addWidget(camera_capture_button);
    QHBoxLayout* bitrait_layout = new QHBoxLayout;
    QLabel* bitrait_label = new QLabel("Set video bitrait: ");
    bitrait_layout->addWidget(bitrait_label);
    QSlider* bitrait_slider = new QSlider(Qt::Horizontal);  // Creating volume slider.
    bitrait_slider->setFixedWidth(160 * scale);
    bitrait_slider->setMinimum(100);
    bitrait_slider->setMaximum(1000);
    bitrait_layout->addWidget(bitrait_slider);
    QSpinBox* bitrait_box = new QSpinBox();  // Creating volume level label.
    bitrait_box->setFixedSize(100 * (scale < 1 ? qSqrt(scale) : scale), 50 * scale);
    bitrait_box->setMaximum(1000);  // some default maximum bitrait
    bitrait_box->setMinimum(100);
    bitrait_layout->addWidget(bitrait_box);
    QPushButton* bitrait_button = new QPushButton("Set");  // Creating audio toggle button.
    bitrait_button->setFixedSize(50 * scale, 50 * scale);
    bitrait_layout->addWidget(bitrait_button);
    connect(bitrait_button, &QPushButton::clicked, this, &DeviceWidget::onSetBitraitCaptureMode);

    connect(bitrait_slider, &QSlider::valueChanged, bitrait_box, &QSpinBox::setValue);  // Connecting slider and label
    connect(bitrait_slider, &QSlider::valueChanged, this, &DeviceWidget::onBitraitChanged);
    connect(bitrait_box, qOverload<int>(&QSpinBox::valueChanged), bitrait_slider, &QSlider::setValue);
    bitrait_slider->setValue(200);
    additional_control_layout->addLayout(bitrait_layout);
    QHBoxLayout* record_layout = new QHBoxLayout;
    QLabel* record_label = new QLabel("Video recording");
    record_layout->addWidget(record_label);
    QPushButton* record_button = new QPushButton;
    record_button->setText("Record");
    record_button->setStyleSheet("QPushButton:checked{ background-color: red }");
    record_button->setCheckable(true);
    connect(record_button, &QPushButton::toggled, this, &DeviceWidget::onToggleRecording);
    record_layout->addWidget(record_button);
    additional_control_layout->addLayout(record_layout);
}

void DeviceWidget::defineWdgets() {
    this->setAlignment(Qt::AlignTop);
    basic_control_layout = new QHBoxLayout;
    setBasicControlLayout();
    this->addLayout(basic_control_layout);
    additional_control_widget = new QWidget;
    additional_control_layout = new QVBoxLayout;
    setAdditionalControlLayout();
    additional_control_widget->setLayout(additional_control_layout);
    this->addWidget(additional_control_widget);
    additional_control_widget->hide();
}

DeviceWidget::DeviceWidget(QString ID_, const QString& name_, const QHostAddress& local_ipv4_address_, qint16 volume_, qreal scale_,
                           QWidget* parent)
    : ID(ID_), name(name_), volume(volume_), scale(scale_), local_ipv4_address(local_ipv4_address_), QVBoxLayout(parent) {
    defineWdgets();
}

DeviceWidget::DeviceWidget(QString ID_, QString&& name_, const QHostAddress& local_ipv4_address_, qint16 volume_, qreal scale_,
                           QWidget* parent)
    : ID(ID_), name(std::move(name_)), volume(volume_), scale(scale_), local_ipv4_address(local_ipv4_address_), QVBoxLayout(parent) {
    defineWdgets();
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
    additional_control_widget->setVisible(expanded_state = !expanded_state);
    qDebug() << expanded_state;
}

void DeviceWidget::onSetScreenCaptureMode() {
    emit sendSetCameraCaptureMode(local_ipv4_address);
}

void DeviceWidget::onSetCameraCaptureMode() {
    emit sendSetScreenCaptureMode(local_ipv4_address);
}

void DeviceWidget::onVolumeChanged(qint16 volume_) {
    volume = volume_;
    emit sendChangeVolume(local_ipv4_address, volume_);
}

void DeviceWidget::onBitraitChanged(qint32 bitrait_) {
    bitrait = bitrait_;
}

void DeviceWidget::onSetBitraitCaptureMode() {
    emit sendChangeVolume(local_ipv4_address, bitrait);
}

void DeviceWidget::onToggleRecording(bool checked) {
    emit sendToggleRecording(local_ipv4_address, checked);
}
