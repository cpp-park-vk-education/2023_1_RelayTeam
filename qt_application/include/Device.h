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

struct DeviceInfo {
	bool audio_state = false;
	bool cast_state = false;
};

class DeviceWidget : public QHBoxLayout {
private:
    Q_OBJECT
	DeviceInfo info;
	QPushButton* audio_button;
	QPushButton* cast_button;
	QPushButton* settings_button;

public:
	DeviceWidget(QString name_ = "", QWidget* parent = nullptr);

private slots:
	void onAudioPressed();

	void onCastPressed();

	void onSettingsPressed();
};
