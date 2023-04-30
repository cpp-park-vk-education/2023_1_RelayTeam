#pragma once

#include "variables.h"
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QString>

class DeviceWidget : public QHBoxLayout {
private:
	Q_OBJECT
	QLabel* name_box;
	QSlider* volume_slider;
	QSpinBox* volume_box;
	QPushButton* audio_button;
	QPushButton* cast_button;
	QPushButton* settings_button;

	bool audio_state = false;
	bool cast_state = false;

	void defineWdgets(qreal scale);

public:
	const QString ID;
	qint16 volume;
	QString name;
	QString ipv6_address;
	QString local_ip;

	explicit DeviceWidget(QString ID_, QString& name_, QString ipv6_address_, qint16 volume_, qreal scale, QString local_ip_ = "",
						  QWidget* parent = nullptr);

	explicit DeviceWidget(QString ID_, QString&& name_, QString ipv6_address_, qint16 volume_, qreal scale, QString local_ip_ = "",
						  QWidget* parent = nullptr);

private slots:
	void onAudioPressed();

	void onCastPressed();

	void onSettingsPressed();

	void onVolumeChanged(qint16 volume_);

signals:
	void sendStartVideoSession(QString local_ip, QString ipv6_address);

	void sendStopVideoSession(QString local_ip);

	void sendStartAudioSession(QString local_ip, QString ipv6_address);

	void sendStopAudioSession(QString local_ip);
};
