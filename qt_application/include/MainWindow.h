#pragma once
#include "DBManager.h"
#include "DeviceWidget.h"
#include <Publisher.h>
#include <QCloseEvent>
#include <QGridLayout>
#include <QGroupBox>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include <SearchWidget.h>

class MainWindow : public QMainWindow {
private:
	Q_OBJECT
	QWidget* main_widget;			   // main  widget
	QGridLayout* main_grid;			   // min widget layout
	QVBoxLayout* devices_layout;	   // layout with device widgets
	QScrollArea* devices_scroll_area;  // scroll area for devices
	QVBoxLayout* left_bar;
	QPushButton* settings_button;
	QPushButton* scan_network_button;
	QTextEdit* input_box;
	SearchWidget* search_w;
	Publisher* publisher_widget;
	bool layout_state = false;

	DBManager data_base;

	void closeEvent(QCloseEvent* event);

public:
	explicit MainWindow(QWidget* parent = nullptr);

private slots:
	void onSettingsButtonPressed();

	void onScanNetworkButtonPressed();
};
