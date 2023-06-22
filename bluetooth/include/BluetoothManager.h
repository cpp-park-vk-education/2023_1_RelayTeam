#pragma once

#include <QLabel>
#include <QListWidget>
#include <QMap>
#include <QPixmap>
#include <QSet>
#include <QShortcut>
#include <QThread>
#include <QTimer>
#include <QtBluetooth/QBluetoothAddress>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothServer>

#include "BluetoothClient.h"
#include "BluetoothScanner.h"
#include "BluetoothServer.h"

class BluetoothManager : public QListWidget {
private:
    Q_OBJECT

    QSet<QString> device_mac_addresses;
    BluetoothServer* bluetooth_server;
    BluetoothClient* bluetooth_client;
    BluetoothScanner* remote_selector;
    QList<QBluetoothHostInfo> local_adapters;
    QString local_name;
    QMap<QBluetoothAddress, QBluetoothServiceInfo> services;

    int current_adapter_index = 0;

    //    int adapterFromUserSelection() const;

    void onConnectToBluetoothService(QBluetoothAddress bluetooth_address);

public:
    explicit BluetoothManager(QWidget* parent = nullptr);

    ~BluetoothManager();

public slots:
    void onStartVideotransmission(QBluetoothAddress bluetooth_address);

    void onStopTransmitt(QBluetoothAddress bluetooth_address);
    // connects to device, requires info about it and sends sendAddDevice
    void onAddButtonCLicked();
    // starts bluetooth video session
    void onStartBluetoothVideoSession(QBluetoothAddress bluetooth_address);
    // kills bluetooth video session
    void onStopBluetoothVideoSession(QBluetoothAddress bluetooth_address);

    void onDeviceMacAddressesUpdated(QSet<QString> device_mac_addresses_);

private slots:
    void onServiceDiscovered(const QBluetoothServiceInfo& service_info);

    void onShowMessage(const QString& sender, const QString& message);

signals:
    // sends info about newly added device (checked with mac address)
    void sendAddDevice(QString name, QBluetoothAddress bluetooth_address, QString mac_address);
    // informs that previusly discovered device became available for connection
    void sendDeviceAvailable(QString mac_address);
    // informs that previusly discovered device became unabailable for connection
    void sendDeviceUnavailable(QString mac_address);

    void onNewAdapterSelected();
};
