#pragma once

#include "DBManager.h"
#include "DeviceWidget.h"
#include "Options.h"
#include "SettingsWidget.h"
#include <Publisher.h>
#include <SearchWidget.h>

#include <QCloseEvent>
#include <QGridLayout>
#include <QGroupBox>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>

class MainWindow : public QMainWindow {
private:
	Q_OBJECT
	// Widgets
	QWidget* main_widget;			   // main  widget
	QHBoxLayout* main_layout;		   // min widget layout
	QVBoxLayout* devices_layout;	   // layout with device widgets
	QScrollArea* devices_scroll_area;  // scroll area for devices
	QVBoxLayout* left_bar;
	QPushButton* settings_button;
	QPushButton* scan_network_button;
	QTextEdit* input_box;
	QLabel* device_name_label;
	QWidget* devices_widget;
	// states
	bool current_search_widget_is_manual;
	bool current_control_widget_is_settings;

	Options* current_options;
	DBManager data_base;
	SearchWidget* search_w;
	Publisher* publisher_widget;
	SettingsWidget* settings_widget;

	void saveAllChanges();

	void closeEvent(QCloseEvent* event);

public:
	explicit MainWindow(QWidget* parent = nullptr);

private slots:
	void onSettingsButtonPressed();

	void onScanNetworkButtonPressed();
};
