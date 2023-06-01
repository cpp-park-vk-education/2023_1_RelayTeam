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
    QRadioButton* screen_capture_button = new QRadioButton;  // Creating screen capture button.
    screen_capture_button->setText("Screen capture");
    screen_capture_button->setChecked(source_mode_is_screen);
    connect(screen_capture_button, &QPushButton::clicked, this, &DeviceWidget::onSetScreenCaptureMode);
    additional_control_layout->addWidget(screen_capture_button);

    QRadioButton* camera_capture_button = new QRadioButton;  // Creating camera capture button.
    camera_capture_button->setText("Camera capture");
    camera_capture_button->setChecked(!source_mode_is_screen);
    connect(camera_capture_button, &QPushButton::clicked, this, &DeviceWidget::onSetCameraCaptureMode);
    additional_control_layout->addWidget(camera_capture_button);

    QRadioButton* both_capture_button = new QRadioButton;  // Creating both capture button.
    both_capture_button->setText("Both capture");
    both_capture_button->setChecked(!source_mode_is_screen);
    connect(both_capture_button, &QPushButton::clicked, this, &DeviceWidget::onSetBothCaptureMode);
    additional_control_layout->addWidget(both_capture_button);

    QHBoxLayout* video_bitrait_layout = new QHBoxLayout;  // Creating bitrait layout.
    video_bitrait_label = new QLabel("Set video bitrait: ");
    video_bitrait_layout->addWidget(video_bitrait_label);
    QSlider* video_bitrait_slider = new QSlider(Qt::Horizontal);
    video_bitrait_slider->setFixedWidth(160 * scale);
    video_bitrait_slider->setMinimum(MINIMUM_VIDEO_BITRAIT);
    video_bitrait_slider->setMaximum(MAXIMUM_VIDEO_BITRAIT);
    video_bitrait_layout->addWidget(video_bitrait_slider);
    QSpinBox* video_bitrait_box = new QSpinBox();
    video_bitrait_box->setFixedSize(100 * (scale < 1 ? qSqrt(scale) : scale), 50 * scale);
    video_bitrait_box->setMinimum(MINIMUM_VIDEO_BITRAIT);
    video_bitrait_box->setMaximum(MAXIMUM_VIDEO_BITRAIT);  // some default maximum bitrait
    video_bitrait_layout->addWidget(video_bitrait_box);
    QPushButton* video_bitrait_button = new QPushButton("Set");
    video_bitrait_button->setFixedSize(50 * scale, 50 * scale);
    video_bitrait_layout->addWidget(video_bitrait_button);
    connect(video_bitrait_button, &QPushButton::clicked, this, &DeviceWidget::onSetVideoBitrait);
    connect(video_bitrait_slider, &QSlider::valueChanged, video_bitrait_box, &QSpinBox::setValue);
    connect(video_bitrait_slider, &QSlider::valueChanged, this, &DeviceWidget::onVideoBitraitChanged);
    connect(video_bitrait_box, qOverload<int>(&QSpinBox::valueChanged), video_bitrait_slider, &QSlider::setValue);
    video_bitrait_slider->setValue(video_bitrait);
    additional_control_layout->addLayout(video_bitrait_layout);

    QHBoxLayout* audio_bitrait_layout = new QHBoxLayout;  // Creating bitrait layout.
    audio_bitrait_label = new QLabel("Set audio bitrait: ");
    audio_bitrait_layout->addWidget(audio_bitrait_label);
    QSlider* audio_bitrait_slider = new QSlider(Qt::Horizontal);
    audio_bitrait_slider->setFixedWidth(160 * scale);
    audio_bitrait_slider->setMinimum(MINIMUM_AUDIO_BITRAIT);
    audio_bitrait_slider->setMaximum(MAXIMUM_AUDIO_BITRAIT);
    audio_bitrait_layout->addWidget(audio_bitrait_slider);
    QSpinBox* audio_bitrait_box = new QSpinBox();
    audio_bitrait_box->setFixedSize(100 * (scale < 1 ? qSqrt(scale) : scale), 50 * scale);
    audio_bitrait_box->setMinimum(MINIMUM_AUDIO_BITRAIT);
    audio_bitrait_box->setMaximum(MAXIMUM_AUDIO_BITRAIT);  // some default maximum bitrait
    audio_bitrait_layout->addWidget(audio_bitrait_box);
    QPushButton* audio_bitrait_button = new QPushButton("Set");
    audio_bitrait_button->setFixedSize(50 * scale, 50 * scale);
    audio_bitrait_layout->addWidget(audio_bitrait_button);
    connect(audio_bitrait_button, &QPushButton::clicked, this, &DeviceWidget::onSetAudioBitrait);
    connect(audio_bitrait_slider, &QSlider::valueChanged, audio_bitrait_box, &QSpinBox::setValue);
    connect(audio_bitrait_slider, &QSlider::valueChanged, this, &DeviceWidget::onAudioBitraitChanged);
    connect(audio_bitrait_box, qOverload<int>(&QSpinBox::valueChanged), audio_bitrait_slider, &QSlider::setValue);
    audio_bitrait_slider->setValue(audio_bitrait);
    additional_control_layout->addLayout(audio_bitrait_layout);

    QHBoxLayout* record_layout = new QHBoxLayout;  // Creating record layout
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
    QFrame* line_start = new QFrame;
    line_start->setFrameShape(QFrame::HLine);
    this->addWidget(line_start);
    setBasicControlLayout();
    this->addLayout(basic_control_layout);
    additional_control_widget = new QWidget;
    additional_control_layout = new QVBoxLayout;
    setAdditionalControlLayout();
    additional_control_widget->setLayout(additional_control_layout);
    this->addWidget(additional_control_widget);
    additional_control_widget->hide();
    QFrame* line_end = new QFrame;
    line_end->setFrameShape(QFrame::HLine);
    this->addWidget(line_end);
}

DeviceWidget::DeviceWidget(QString ID_, const QString& name_, qint16 volume_, qreal scale_, qint32 video_bitrait_, qint32 audio_bitrait_,
                           QWidget* parent)
    : ID(ID_),
      name(name_),
      volume(volume_),
      scale(scale_),
      video_bitrait(video_bitrait_),
      unset_video_bitrait(video_bitrait_),
      audio_bitrait(audio_bitrait_),
      unset_audio_bitrait(audio_bitrait_),
      QVBoxLayout(parent) {
    defineWdgets();
}

DeviceWidget::DeviceWidget(QString ID_, QString&& name_, qint16 volume_, qreal scale_, qint32 video_bitrait_, qint32 audio_bitrait_,
                           QWidget* parent)
    : ID(ID_),
      name(std::move(name_)),
      volume(volume_),
      scale(scale_),
      video_bitrait(video_bitrait_),
      unset_video_bitrait(video_bitrait_),
      audio_bitrait(audio_bitrait_),
      unset_audio_bitrait(audio_bitrait_),
      QVBoxLayout(parent) {
    defineWdgets();
}

void DeviceWidget::setLocalIPv4(const QHostAddress& local_ipv4_address_) {
    local_ipv4_address = local_ipv4_address_;
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
    if (expanded_state) {
        additional_control_widget->hide();
        settings_button->setStyleSheet("QPushButton{ background-color: white }");
    } else {
        additional_control_widget->show();
        settings_button->setStyleSheet("QPushButton{ background-color: blue }");
    }
    expanded_state = !expanded_state;
}

void DeviceWidget::onSetScreenCaptureMode() {
    emit sendSetCameraCaptureMode(local_ipv4_address);
}

void DeviceWidget::onSetCameraCaptureMode() {
    emit sendSetScreenCaptureMode(local_ipv4_address);
}

void DeviceWidget::onSetBothCaptureMode() {
    emit sendSetBothCaptureMode(local_ipv4_address);
}

void DeviceWidget::onVolumeChanged(qint16 volume_) {
    volume = volume_;
    emit sendChangeVolume(local_ipv4_address, volume_);
}

void DeviceWidget::onVideoBitraitChanged(qint32 video_bitrait_) {
    unset_video_bitrait = video_bitrait_;

    if (video_bitrait_ != video_bitrait) {
        video_bitrait_label->setStyleSheet("QLabel { color : red; }");
    } else {
        video_bitrait_label->setStyleSheet("QLabel { color : grey; }");
    }
}

void DeviceWidget::onAudioBitraitChanged(qint32 audio_bitrait_) {
    unset_audio_bitrait = audio_bitrait_;
    if (audio_bitrait_ != audio_bitrait) {
        audio_bitrait_label->setStyleSheet("QLabel { color : red; }");
    } else {
        audio_bitrait_label->setStyleSheet("QLabel { color : grey; }");
    }
}

void DeviceWidget::onSetVideoBitrait() {
    video_bitrait = unset_video_bitrait;
    emit sendChangeVolume(local_ipv4_address, video_bitrait);
    video_bitrait_label->setStyleSheet("QLabel { color : grey; }");
}

void DeviceWidget::onSetAudioBitrait() {
    audio_bitrait = unset_audio_bitrait;
    emit sendChangeVolume(local_ipv4_address, audio_bitrait);
    audio_bitrait_label->setStyleSheet("QLabel { color : grey; }");
}

void DeviceWidget::onToggleRecording(bool checked) {
    emit sendToggleRecording(local_ipv4_address, checked);
}
