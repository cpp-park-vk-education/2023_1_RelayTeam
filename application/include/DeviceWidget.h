#pragma once

#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QString>
#include <QtNetwork/QHostAddress>

#include "variables.h"

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
	QHostAddress local_ipv4_address;

	explicit DeviceWidget(QString ID_, const QString& name_, const QHostAddress& local_ipv4_address_, qint16 volume_, qreal scale,
						  QWidget* parent = nullptr);

	explicit DeviceWidget(QString ID_, QString&& name_, const QHostAddress& local_ipv4_address_, qint16 volume_, qreal scale,
						  QWidget* parent = nullptr);

private slots:
	void onAudioPressed();

	void onCastPressed();

	void onSettingsPressed();

	void onVolumeChanged(qint16 volume_);

signals:
	void sendStartVideoSession(QHostAddress ipv6_address);

	void sendStopVideoSession(QHostAddress ipv6_address);

	void sendStartAudioSession(QHostAddress ipv6_address);

	void sendStopAudioSession(QHostAddress ipv6_address);
};
