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
	bool audio_state = false;
	bool cast_state = false;

	void defineWdgets();

public:
	const int ID;
	int volume;
	QString name;

	explicit DeviceWidget(int ID_, QString& name_, int volume_, QWidget* parent = nullptr);

	explicit DeviceWidget(int ID_, QString&& name_, int volume_, QWidget* parent = nullptr);

private slots:
	void onAudioPressed();

	void onCastPressed();

	void onSettingsPressed();

	void onVolumeChanged(int volume_);
};
