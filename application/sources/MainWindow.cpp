#include "MainWindow.h"

#include <UITools.h>

#include <QMessageBox>
#include <QSlider>
#include <iostream>

void MainWindow::createRightBar() {
    right_bar = new QVBoxLayout();                  // Creating right bar.
    settings_button = new QPushButton("Settings");  // Creating settings button.
    settings_button->setFixedHeight(60 * options->getScale());
    connect(settings_button, &QPushButton::clicked, this, &MainWindow::onSettingsButtonPressed);
    right_bar->addWidget(settings_button);
    devices_layout = new QVBoxLayout();  // Creating devices layout with scroll area.
    data_base.getDevices(devices_layout, options->getScale());
    for (size_t i = 0; i < devices_layout->count(); ++i) {
        makeDeviceConnections(static_cast<DeviceWidget*>(devices_layout->itemAt(i)));
    }
    devices_layout->setContentsMargins(0, 0, 0, 0);
    devices_widget = new QWidget();  // Creating widget for devices layout
    devices_widget->sizeHint();
    devices_widget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Maximum);
    devices_widget->setLayout(devices_layout);
    devices_scroll_area = new QScrollArea();  // Creating and configuring scroll area for devices widget.
    devices_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    devices_scroll_area->setWidgetResizable(true);
    devices_scroll_area->setAlignment(Qt::AlignRight);
    devices_scroll_area->setWidget(devices_widget);
    right_bar->addWidget(devices_scroll_area);
    settings_widget = new SettingsWidget(options);  // Creating settings widget.
    settings_widget->setFixedWidth(570 * options->getScale());
    settings_widget->hide();
    right_bar->addWidget(settings_widget);
    right_bar_widget = new QWidget;
    right_bar_widget->setLayout(right_bar);
    right_bar_widget->setMinimumWidth(600 * options->getScale());
}

void MainWindow::createLeftBar() {
    left_bar = new QVBoxLayout();  // creating left bar
    left_bar->setAlignment(Qt::AlignLeft);
    button_layout = new QHBoxLayout();                    // Creating button layout.
    scan_network_button = new QPushButton("Scan local");  // Creating scan local button.
    scan_network_button->setFixedHeight(60 * options->getScale());
    connect(scan_network_button, &QPushButton::clicked, this, &MainWindow::onScanNetworkButtonPressed);
    button_layout->addWidget(scan_network_button);
    add_button = new QPushButton("Add");  // Creatint add button
    add_button->setFixedHeight(60 * options->getScale());
    button_layout->addWidget(add_button);
    left_bar->addLayout(button_layout);

    bluetooth_manager = new BluetoothManager;  // Creating input box.
    left_bar->addWidget(bluetooth_manager);
    connect(add_button, &QPushButton::clicked, bluetooth_manager, &BluetoothManager::onAddButtonCLicked);

    search_widget = new SearchWidget(options->getScale());  // Creating network widgets.
    search_widget->hide();
    left_bar->addWidget(search_widget);
    connect(add_button, &QPushButton::clicked, search_widget, &SearchWidget::onAddButtonCLicked);
    connect(search_widget, &SearchWidget::devicePreparedToAdd, this, &MainWindow::onDevicePreparedToAdd);
    connect(search_widget, &SearchWidget::sendUpdateAddress, this, &MainWindow::onUpdateAddress);
    publisher_widget = new Publisher(options->device_name, this);
    connect(this, &MainWindow::sendDeviceMacAddressesUpdated, search_widget, &SearchWidget::onDeviceIdsUpdated);
    connect(settings_widget, &SettingsWidget::sendChangeServiceName, publisher_widget, &Publisher::onChangeServiceName);

    left_bar_widget = new QWidget;
    left_bar_widget->setLayout(left_bar);
    left_bar_widget->setMinimumWidth(300 * options->getScale());
}

MainWindow::MainWindow(QScreen* application_screen_, QWidget* parent)
    : application_screen(application_screen_),
      QMainWindow(parent),
      current_search_widget_is_bluetooth(true),
      current_control_widget_is_settings(false),
      streaming_session_manager() {
    qDebug() << "MR: MainWindow initialization started";
    qDebug() << application_screen->size();
    qDebug() << application_screen->orientation();

    //    QStackedWidget* m_stackedWidget;
    options = new Options();
    data_base.getOptions(options);
    qint32 max_screen_dimention = qMax(application_screen->size().width(), application_screen->size().height());
    if (application_screen->orientation() & (Qt::LandscapeOrientation | Qt::InvertedLandscapeOrientation)) {
        if (options->scale_factor > max_screen_dimention * 100 / 930) {
            settings_widget->setScale(max_screen_dimention * 100 / 930);
        }
    } else {
        if (options->scale_factor > max_screen_dimention * 100 / 630) {
            settings_widget->setScale(max_screen_dimention * 100 / 630);
        }
    }

    QFont font = this->font();
    font.setPointSize(16 * getFontScaling(options->getScale()));
    this->setFont(font);
    main_widget = new QWidget();  // Setting up main window widgets.
    main_layout = new QGridLayout();
    main_widget->setLayout(main_layout);
    this->setCentralWidget(main_widget);
    // Creating main widget layouts
    createRightBar();
    createLeftBar();

    main_layout->addWidget(left_bar_widget, 0, 0);
    if (application_screen->orientation() & (Qt::LandscapeOrientation | Qt::InvertedLandscapeOrientation)) {
        main_layout->addWidget(right_bar_widget, 0, 1);
        main_widget->setMinimumSize(930 * options->getScale(), 400 * options->getScale());
        qDebug() << "MR: 1, 0";
    } else {
        main_layout->addWidget(right_bar_widget, 1, 0);
        main_widget->setMinimumSize(630 * options->getScale(), 400 * options->getScale());
        qDebug() << "MR: 0, 1";
    }

    getDeviceMacAddresses();  // Get device mac addresses and send them to classes that require them.

    main_widget->show();

    // Initializing ssl server
    ssl_io_manager = new SslIOManager;
    QThread* ssl_io_manager_thread = new QThread;

    ssl_io_manager->moveToThread(ssl_io_manager_thread);
    ssl_io_manager_thread->start();

    connect(this, &MainWindow::killAll, ssl_io_manager_thread, &QThread::deleteLater);
    connect(ssl_io_manager, &SslIOManager::sendReceivedPorts, &streaming_session_manager, &SessionManager::onReceivedPorts);
    connect(&streaming_session_manager, &SessionManager::sendStartReciver, ssl_io_manager, &SslIOManager::onStartReciver);
    connect(ssl_io_manager, &SslIOManager::sendStartReceivingSession, &streaming_session_manager, &SessionManager::onStartReceivingSession);
    connect(&streaming_session_manager, &SessionManager::sendSetPorts, ssl_io_manager, &SslIOManager::onSendPorts);
}

MainWindow::~MainWindow() {
    emit killAll();
    delete options;
}

void MainWindow::makeDeviceConnections(DeviceWidget* device) {
    connect(device, &DeviceWidget::sendStartVideoSession, &streaming_session_manager, &SessionManager::onStartVideoSession);
    connect(device, &DeviceWidget::sendStopVideoSession, &streaming_session_manager, &SessionManager::onKillVideoSession);
    connect(device, &DeviceWidget::sendStartAudioSession, &streaming_session_manager, &SessionManager::onStartAudioSession);
    connect(device, &DeviceWidget::sendStopAudioSession, &streaming_session_manager, &SessionManager::onKillAudioSession);

    //    connect(devoce, &DeviceWidget::sendChangeVideoBitrait, &streaming_session_manager, &SessionManager::);
    //    connect(devoce, &DeviceWidget::sendChangeAudioBitrait, &streaming_session_manager, &SessionManager::);
    //    connect(devoce, &DeviceWidget::sendChangeVolume, &streaming_session_manager, &SessionManager::);
    //    connect(devoce, &DeviceWidget::sendSetCameraCaptureMode, &streaming_session_manager, &SessionManager::);
    //    connect(devoce, &DeviceWidget::sendSetScreenCaptureMode, &streaming_session_manager, &SessionManager::);
    //    connect(devoce, &DeviceWidget::sendSetBothCaptureMode, &streaming_session_manager, &SessionManager::);
    //    connect(devoce, &DeviceWidget::sendToggleRecording, &streaming_session_manager, &SessionManager::);
}

void MainWindow::onSettingsButtonPressed() {
    if (current_control_widget_is_settings) {
        settings_widget->hide();
        devices_scroll_area->show();
        settings_button->setText("Settings");
    } else {
        settings_widget->show();
        devices_scroll_area->hide();
        settings_button->setText("View Devices");
    }
    current_control_widget_is_settings = !current_control_widget_is_settings;
}

void MainWindow::onScanNetworkButtonPressed() {
    if (current_search_widget_is_bluetooth) {
        bluetooth_manager->clearSelection();
        bluetooth_manager->hide();
        search_widget->show();
        scan_network_button->setText("Bluetooth");
    } else {
        search_widget->clearSelection();
        search_widget->hide();
        bluetooth_manager->show();
        scan_network_button->setText("Scan local");
    }
    current_search_widget_is_bluetooth = !current_search_widget_is_bluetooth;
}

void MainWindow::saveAllChanges() {
    for (size_t i = 0; i < devices_layout->count(); ++i) {
        data_base.saveDeviceChanges(static_cast<DeviceWidget*>(devices_layout->itemAt(i)));
    }
    data_base.saveOptionsChanges(options);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    saveAllChanges();
    QMessageBox::StandardButton resBtn =
        QMessageBox::question(this, "Warning", tr("Exit the application?\n"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::onDevicePreparedToAdd(QString name, QHostAddress local_ipv4_address, QString mac_address) {
    qDebug() << "Adding device: " << name;
    DeviceWidget* device_widget = new DeviceWidget(mac_address, name, 50, options->getScale());
    device_widget->setLocalIPv4(local_ipv4_address);
    makeDeviceConnections(device_widget);
    devices_layout->addLayout(device_widget);
    data_base.addDevice(device_widget);
    device_mac_addresses.insert(mac_address);
    emit sendDeviceMacAddressesUpdated(device_mac_addresses);
}

void MainWindow::getDeviceMacAddresses() {
    for (size_t i = 0; i < devices_layout->count(); ++i) {
        device_mac_addresses.insert(static_cast<DeviceWidget*>(devices_layout->itemAt(i))->ID);
    }
    emit sendDeviceMacAddressesUpdated(device_mac_addresses);
}

void MainWindow::onUpdateAddress(QString mac_address, QHostAddress local_ipv4_address) {
    for (size_t i = 0; i < devices_layout->count(); ++i) {
        if (static_cast<DeviceWidget*>(devices_layout->itemAt(i))->ID == mac_address) {
            qDebug() << "Updating local ipv4 for" << static_cast<DeviceWidget*>(devices_layout->itemAt(i))->name;
            static_cast<DeviceWidget*>(devices_layout->itemAt(i))->local_ipv4_address = local_ipv4_address;
            return;
        }
    }
}
