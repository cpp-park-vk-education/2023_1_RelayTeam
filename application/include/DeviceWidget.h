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
#include <QtBluetooth/QBluetoothAddress>
#include <QtNetwork/QHostAddress>

#include "variables.h"

class DeviceWidget : public QVBoxLayout {
private:
    Q_OBJECT

    enum CaptureModes { Camera, Screen, Both, Bluetooth };

    /////////////////////////////////////////////////
    QLabel* wifi_label;
    QLabel* bluetooth_label;
    QLabel* name_box;
    QSlider* volume_slider;
    QSpinBox* volume_box;
    QPushButton* audio_button;
    QPushButton* cast_button;
    QPushButton* settings_button;
    QHBoxLayout* basic_control_layout;
    /////////////////////////////////////////////////
    QLabel* video_bitrait_label;
    QLabel* audio_bitrait_label;
    /////////////////////////////////////////////////
    QWidget* additional_control_widget;
    QVBoxLayout* additional_control_layout;

    qint32 unset_video_bitrait;
    qint32 unset_audio_bitrait;

    qreal scale;

    CaptureModes capture_mode = CaptureModes::Camera;
    bool wifi_state = false;
    bool bluetooth_state = false;
    bool expanded_state = false;
    bool audio_state = false;
    bool cast_state = false;
    bool bluetooth_cast_state = false;

    QBluetoothAddress bluetooth_address = QBluetoothAddress();  // isNull = true

    QHostAddress local_ipv4_address = QHostAddress();  // isNull = true

    void setBasicControlLayout();

    void setAdditionalControlLayout();

    void defineWdgets();

public:
    const QString ID;
    qint16 volume;
    QString name;
    qint32 video_bitrait;
    qint32 audio_bitrait;
    bool source_mode_is_screen = true;

    explicit DeviceWidget(QString ID_, const QString& name_, qint16 volume_, qreal scale, qint32 video_bitrait_ = DEFAULT_VIDEO_BITRAIT,
                          qint32 audio_bitrait_ = DEFAULT_AUDIO_BITRAIT, QWidget* parent = nullptr);

    explicit DeviceWidget(QString ID_, QString&& name_, qint16 volume_, qreal scale_, qint32 video_bitrait_ = DEFAULT_VIDEO_BITRAIT,
                          qint32 audio_bitrait_ = DEFAULT_AUDIO_BITRAIT, QWidget* parent = nullptr);

    void setLocalIPv4(const QHostAddress& local_ipv4_address_);

    void unsetLocalIPv4();

    void setBluetoothAddress(const QBluetoothAddress& bluetooth_address_);

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

    void onSetBluetoothCaptureMode();

    void onToggleRecording(bool checked);

signals:
    /////////////////SESSION_CONTROL//////////////////////////////////////////
    void sendStartVideoSession(QHostAddress ipv4_address);

    void sendStopVideoSession(QHostAddress ipv4_address);

    void sendStartAudioSession(QHostAddress ipv4_address);

    void sendStopAudioSession(QHostAddress ipv4_address);

    void sendStartBluetoothVideoSession(QBluetoothAddress bluetooth_address);

    void sendStopBluetoothVideoSession(QBluetoothAddress bluetooth_address);
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
