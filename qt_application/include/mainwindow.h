#pragma once
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QSlider>
#include <QTextEdit>
#include "Device.h"
#include <list>
#include <string>

class MainWindow : public QMainWindow {
private:
	Q_OBJECT
	QWidget* main_widget = new QWidget();			 // main  widget
	QGridLayout* main_grid = new QGridLayout(this);	 // min widget layout
	std::list<DeviceWidget*> device_list;			 // list of device widgets

	void getDevices();

public:
	explicit MainWindow(QWidget* parent = nullptr);

private slots:
	void onSettingsButtonPressed();

	void onScanNetworkButtonPressed();
};
