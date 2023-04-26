#pragma once

#include <Options.h>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class SettingsWidget : public QWidget {
private:
	Q_OBJECT
	Options* options;
	QVBoxLayout* main_layout;
	QPushButton* set_device_name_button;
	QPushButton* reset_device_name_button;
	QLineEdit* device_name_input_box;
	QHBoxLayout* change_device_name_layout;
	QHBoxLayout* change_scale_layout;
	QSlider* scale_slider;
	QSpinBox* scale_box;
	QLabel* scaling_label;
	QLabel* scaling_warning_label;

public:
	SettingsWidget(Options* options_, QWidget* parrent = nullptr);

private slots:
	void onScaleChanged(int volume_);

public slots:
	void onSetDeviceNameButtonClicked();

	void onResetDeviceNameButtonClicked();

signals:
	void sendChangeServiceName(const QString& service_name);
};
