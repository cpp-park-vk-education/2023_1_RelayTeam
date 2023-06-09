#pragma once

#include <Publisher.h>
#include <SearchWidget.h>
#include <SessionManager.h>
#include <SslIOManager.h>

#include <QCloseEvent>
#include <QGestureEvent>
#include <QGridLayout>
#include <QGroupBox>
// #include <QJniObject>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QScroller>
#include <QSet>
#include <QStackedWidget>
#include <QTextEdit>
#include <QThread>
#include <QWindow>
#include <QtBluetooth/QBluetoothAddress>
#include <QtNetwork/QHostAddress>

#include "BluetoothManager.h"
#include "DBManager.h"
#include "DeviceWidget.h"
#include "Options.h"
#include "SettingsWidget.h"

class MainWindow : public QMainWindow {
private:
    Q_OBJECT
    // Widgets
    QWidget* main_widget;      // main  widget
    QGridLayout* main_layout;  // min widget layout
    // right bar widgets
    QWidget* right_bar_widget;
    QVBoxLayout* right_bar;
    QPushButton* settings_button;
    QWidget* devices_widget;
    QVBoxLayout* devices_layout;       // layout with device widgets
    QScrollArea* devices_scroll_area;  // scroll area for devices
    // left bar widgets
    QWidget* left_bar_widget;
    QVBoxLayout* left_bar;
    QHBoxLayout* button_layout;
    QPushButton* scan_network_button;
    QPushButton* add_button;
    QStackedWidget* main_window_stack_widget;
    BluetoothManager* bluetooth_manager;

    QLabel* stacked_widget_placeholder;

    SearchWidget* search_widget;
    Publisher* publisher_widget;

    // states
    bool current_search_widget_is_bluetooth;
    bool current_control_widget_is_settings;

    bool is_dragging_mouse;
    QPoint mouse_drag_start_pos;
    QScreen* application_screen;
    Options* options;
    SettingsWidget* settings_widget;
    QSet<QString> device_mac_addresses;

    // NonWidgetRelientInterractions
    DBManager data_base;
    SessionManager streaming_session_manager;

    SslIOManager* ssl_io_manager;

    void createRightBar();

    void createLeftBar();

    void saveAllChanges();

    void closeEvent(QCloseEvent* event) override;

    void getDeviceMacAddresses();

    void makeDeviceConnections(DeviceWidget* device);

private:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow();

private slots:
    void onSettingsButtonPressed();

    void onScanNetworkButtonPressed();

public slots:
    void onDevicePreparedToAdd(QString name, QHostAddress ipv6_address, QString mac_address);

    void onBluetoothDevicePreparedToAdd(QString name, QBluetoothAddress local_ipv4_address, QString mac_address);

    void onUpdateAddress(QString mac_address, QHostAddress ipv6_address);

    void onUnsetAddress(QString mac_address);

signals:
    void sendDeviceMacAddressesUpdated(QSet<QString> device_ids);

    void killAll();
};
