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
	QPushButton* audio_button;
	QPushButton* cast_button;
	QPushButton* settings_button;
	QLabel* name_box;
	QSlider* volume_slider;
	QSpinBox* volume_box;
	bool audio_state = false;
	bool cast_state = false;

	void defineWdgets(qreal scale);

public:
	const qint32 ID;
	qint16 volume;
	QString name;

	explicit DeviceWidget(qint32 ID_, QString& name_, qint16 volume_, qreal scale, QWidget* parent = nullptr);

	explicit DeviceWidget(qint32 ID_, QString&& name_, qint16 volume_, qreal scale, QWidget* parent = nullptr);

private slots:
	void onAudioPressed();

	void onCastPressed();

	void onSettingsPressed();

	void onVolumeChanged(qint16 volume_);
};
