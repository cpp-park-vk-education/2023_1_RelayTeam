#pragma once

#include <QLabel>
#include <QListWidget>
#include <QMap>
#include <QPixmap>
#include <QShortcut>
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

signals:
    void sendMessage(const QString& data);

public slots:
    void onStartVideotransmission(const QBluetoothAddress& bluetooth_address) {}

    void onStopTransmitt(const QBluetoothAddress& bluetooth_address) {}

    void onAddButtonCLicked() {}

private slots:
    void onServiceDiscovered(const QBluetoothServiceInfo& service_info);

    void onShowMessage(const QString& sender, const QString& message);

    //    void onNewAdapterSelected();
};
