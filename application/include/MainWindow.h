#pragma once

#include <Publisher.h>
#include <SearchWidget.h>
#include <SessionManager.h>
#include <SslIOManager.h>

#include <QCloseEvent>
#include <QGridLayout>
#include <QGroupBox>
#include <QMainWindow>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QSet>
#include <QStackedWidget>
#include <QTextEdit>
#include <QThread>
#include <QWindow>
#include <QtNetwork/QHostAddress>

#include "DBManager.h"
#include "DeviceWidget.h"
#include "Options.h"
#include "SettingsWidget.h"

class MainWindow : public QMainWindow {
private:
    Q_OBJECT
    // Widgets
    QWidget* main_widget;         // main  widget
    QGridLayout* main_layout;     // min widget layout
    QVBoxLayout* devices_layout;  // layout with device widgets
    QVBoxLayout* left_bar;
    QVBoxLayout* right_bar;
    QHBoxLayout* button_layout;
    QScrollArea* devices_scroll_area;  // scroll area for devices
    QPushButton* settings_button;
    QPushButton* scan_network_button;
    QPushButton* add_button;
    QTextEdit* input_box;
    QLabel* device_name_label;
    QWidget* devices_widget;
    // states
    bool current_search_widget_is_manual;
    bool current_control_widget_is_settings;

    QScreen* application_screen;
    Options* options;
    DBManager data_base;
    SettingsWidget* settings_widget;
    QSet<QString> device_ids;

    SearchWidget* search_widget;
    Publisher* publisher_widget;

    SessionManager streaming_session_manager;

    SslIOManager* ssl_io_manager;

    void createRightBar();

    void createLeftBar();

    void saveAllChanges();

    void closeEvent(QCloseEvent* event);

    void getDeviceIds();

    void makeDeviceConnections(DeviceWidget* device);

public:
    explicit MainWindow(QScreen* application_screen_, QWidget* parent = nullptr);

    ~MainWindow();

private slots:
    void onSettingsButtonPressed();

    void onScanNetworkButtonPressed();

public slots:
    void onDevicePreparedToAdd(QString name, QHostAddress ipv6_address, QString mac_address);

    void onUpdateAddress(QString mac_address, QHostAddress ipv6_address);

signals:
    void sendDeviceIdsUpdated(QSet<QString> device_ids);

    void killAll();
};
