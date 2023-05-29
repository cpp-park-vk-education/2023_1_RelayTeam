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
    QHostAddress local_ipv4_address;
    qint32 bitrait = 200;
    bool source_mode_is_screen = true;

    explicit DeviceWidget(QString ID_, const QString& name_, const QHostAddress& local_ipv4_address_, qint16 volume_, qreal scale_,
                          QWidget* parent = nullptr);

    explicit DeviceWidget(QString ID_, QString&& name_, const QHostAddress& local_ipv4_address_, qint16 volume_, qreal scale_,
                          QWidget* parent = nullptr);

private slots:
    void onAudioPressed();

    void onCastPressed();

    void onSettingsPressed();

    void onVolumeChanged(qint16 volume_);

    void onBitraitChanged(qint32 bitrait_);

    void onSetScreenCaptureMode();

    void onSetCameraCaptureMode();

    void onSetBitraitCaptureMode();

    void onToggleRecording(bool checked);

signals:
    void sendStartVideoSession(QHostAddress ipv4_address);

    void sendStopVideoSession(QHostAddress ipv4_address);

    void sendStartAudioSession(QHostAddress ipv4_address);

    void sendStopAudioSession(QHostAddress ipv4_address);

    void sendChangeVolume(QHostAddress ipv4_address, qint16 volume_);

    void sendChangeBitrait(QHostAddress ipv4_address, qint16 volume_);

    void sendSetScreenCaptureMode(QHostAddress ipv4_address);

    void sendSetCameraCaptureMode(QHostAddress ipv4_address);

    void sendToggleRecording(QHostAddress ipv4_address, bool checked);
};
