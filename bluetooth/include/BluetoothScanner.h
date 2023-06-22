#pragma once

#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QtBluetooth/QBluetoothAddress>
#include <QtBluetooth/QBluetoothServiceDiscoveryAgent>
#include <QtBluetooth/QBluetoothServiceInfo>
#include <QtBluetooth/QBluetoothUuid>

class BluetoothScanner : public QObject {
private:
    Q_OBJECT

    QBluetoothServiceDiscoveryAgent* m_discovery_agent;
    QBluetoothServiceInfo m_service;
    QMap<QString, QBluetoothServiceInfo> m_discovered_services;

public:
    explicit BluetoothScanner(const QBluetoothAddress& localAdapter, QWidget* parent = nullptr);

    ~BluetoothScanner();

    void startDiscovery(const QBluetoothUuid& uuid);

    QBluetoothServiceInfo getServiceInfo(const QString& service_name);

private slots:
    void onServiceDiscovered(const QBluetoothServiceInfo& service_info);

    void onDiscoveryFinished();

    void onDiscoveryCanceled();

signals:
    void sendAddService(const QBluetoothServiceInfo& service_info);
};
