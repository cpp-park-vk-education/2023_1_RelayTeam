#include "BluetoothScanner.h"

#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothLocalDevice>

#include "variables.h"

BluetoothScanner::BluetoothScanner(const QBluetoothAddress& local_adapter, QWidget* parent)
    : QObject(parent), m_discovery_agent(new QBluetoothServiceDiscoveryAgent(local_adapter)) {
    connect(m_discovery_agent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered, this, &BluetoothScanner::onServiceDiscovered);
    connect(m_discovery_agent, &QBluetoothServiceDiscoveryAgent::finished, this, &BluetoothScanner::onDiscoveryFinished);
    connect(m_discovery_agent, &QBluetoothServiceDiscoveryAgent::canceled, this, &BluetoothScanner::onDiscoveryCanceled);
}

BluetoothScanner::~BluetoothScanner() {
    delete m_discovery_agent;
}

void BluetoothScanner::startDiscovery(const QBluetoothUuid& uuid) {
    qDebug() << "Starting bluetooth scanner";
    m_discovery_agent->setUuidFilter(uuid);
    m_discovery_agent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
}

void BluetoothScanner::onServiceDiscovered(const QBluetoothServiceInfo& service_info) {
    qDebug() << "Discovered bluetooth service: address: " << service_info.device().address()
             << " address: " << service_info.device().name();

    emit sendAddService(service_info);
}

void BluetoothScanner::onDiscoveryFinished() {
    qDebug() << "Bluetooth socket finished scanning";
}

void BluetoothScanner::onDiscoveryCanceled() {
    qDebug() << "Bluetooth socket canceled scanning";
}
