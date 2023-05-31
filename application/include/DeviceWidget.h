#pragma once

#include <QDir>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <QtNetwork/QHostAddress>

#include "variables.h"

class DeviceWidget : public QVBoxLayout {
private:
    Q_OBJECT
    QLabel* name_box;
    QSlider* volume_slider;
    QSpinBox* volume_box;
    QPushButton* audio_button;
    QPushButton* cast_button;
    QPushButton* settings_button;
    QHBoxLayout* basic_control_layout;

    QWidget* additional_control_widget;
    QVBoxLayout* additional_control_layout;

    qreal scale;

    bool expanded_state = false;
    bool audio_state = false;
    bool cast_state = false;

    void setBasicControlLayout();

    void setAdditionalControlLayout();

    void defineWdgets();

public:
    const QString ID;
    qint16 volume;
    QString name;
    QHostAddress local_ipv4_address = QHostAddress();  // isNull = true
    qint32 video_bitrait;
    qint32 audio_bitrait;
    bool source_mode_is_screen = true;

    explicit DeviceWidget(QString ID_, const QString& name_, qint16 volume_, qreal scale, qint32 video_bitrait_ = DEFAULT_VIDEO_BITRAIT,
                          qint32 audio_bitrait_ = DEFAULT_AUDIO_BITRAIT, QWidget* parent = nullptr);

    explicit DeviceWidget(QString ID_, QString&& name_, qint16 volume_, qreal scale_, qint32 video_bitrait_ = DEFAULT_VIDEO_BITRAIT,
                          qint32 audio_bitrait_ = DEFAULT_AUDIO_BITRAIT, QWidget* parent = nullptr);

    void setLocalIPv4(const QHostAddress& local_ipv4_address_);

private slots:
    void onAudioPressed();

    void onCastPressed();

    void onSettingsPressed();

    void onVolumeChanged(qint16 volume_);
    /////////////////////////////////////////////////////////////////////////
    void onVideoBitraitChanged(qint32 bitrait_);

    void onAudioBitraitChanged(qint32 bitrait_);

    void onSetVideoBitrait();

    void onSetAudioBitrait();
    /////////////////////////////////////////////////////////////////////////
    void onSetScreenCaptureMode();

    void onSetCameraCaptureMode();

    void onSetBothCaptureMode();

    void onToggleRecording(bool checked);

signals:
    /////////////////VIDEO_SESSION_CONTROL///////////////////////////////////
    void sendStartVideoSession(QHostAddress ipv4_address);

    void sendStopVideoSession(QHostAddress ipv4_address);

    void sendStartAudioSession(QHostAddress ipv4_address);

    void sendStopAudioSession(QHostAddress ipv4_address);
    /////////////////VOLUME_CONTROL//////////////////////////////////////////
    void sendChangeVolume(QHostAddress ipv4_address, qint16 volume_);
    /////////////////BITRAIT_CONTROL/////////////////////////////////////////
    void sendChangeVideoBitrait(QHostAddress ipv4_address, qint16 volume_);

    void sendChangeAudioBitrait(QHostAddress ipv4_address, qint16 volume_);
    /////////////////CAPTURE_MODE_CONTROL////////////////////////////////////
    void sendSetScreenCaptureMode(QHostAddress ipv4_address);

    void sendSetCameraCaptureMode(QHostAddress ipv4_address);

    void sendSetBothCaptureMode(QHostAddress ipv4_address);
    /////////////////RECORDING_CONTROL///////////////////////////////////////
    void sendToggleRecording(QHostAddress ipv4_address, bool checked);
};
