#pragma once
#include "DBManager.h"
#include "DeviceWidget.h"
#include <QCloseEvent>
#include <QGridLayout>
#include <QGroupBox>
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
private:
	Q_OBJECT
	QWidget* main_widget;		  // main  widget
	QGridLayout* main_grid;		  // min widget layout
	QVBoxLayout* devices_layout;  // layout with device widgets
	DBManager data_base;

	void closeEvent(QCloseEvent* event);

public:
	explicit MainWindow(QWidget* parent = nullptr);

private slots:
	void onSettingsButtonPressed();

	void onScanNetworkButtonPressed();
};
