#pragma once

#include <Options.h>

#include <QLineEdit>
#include <QPushButton>
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

public:
	SettingsWidget(Options* options_, QWidget* parrent = nullptr);

public slots:
	void onSetDeviceNameButtonClicked();

	void onResetDeviceNameButtonClicked();
signals:
	void sendChangeServiceName(const QString& service_name);
};
