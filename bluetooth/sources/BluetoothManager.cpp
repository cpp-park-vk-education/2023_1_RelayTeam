#include "BluetoothManager.h"

#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QLabel>
#include <QPixmap>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothUuid>

#include "variables.h"

BluetoothManager::BluetoothManager(QWidget* parent) : QListWidget(parent) {
    //    connect(/* твоя кнопка */, &QPushButton::clicked, this, &Manager::onConnectClicked);
    //    connect(/* твоя кнопка */, &QPushButton::clicked, this, &Manager::onSendStarted);

    local_adapters = QBluetoothLocalDevice::allDevices();
    if (local_adapters.count() < 2) {
        qDebug() << "localAdapters 0";
    } else {
        QBluetoothLocalDevice adapter(local_adapters.at(0).address());
        adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    }

    QThread* bluetooth_client_thread = new QThread;
    bluetooth_client = new BluetoothClient();
    bluetooth_client->moveToThread(bluetooth_client_thread);

    QThread* bluetooth_server_thread = new QThread;
    bluetooth_server = new BluetoothServer();
    bluetooth_server->moveToThread(bluetooth_server_thread);
    QMetaObject::invokeMethod(bluetooth_server, "initServer", Qt::QueuedConnection);
    connect(bluetooth_server, &BluetoothServer::sendMessageReceived, this, &BluetoothManager::onShowMessage);

    local_name = QBluetoothLocalDevice().name();

    const QBluetoothAddress adapter = local_adapters.isEmpty() ? QBluetoothAddress() : local_adapters.at(current_adapter_index).address();
    remote_selector = new BluetoothScanner(adapter);
#ifdef Q_OS_ANDROID
    if (QtAndroid::androidSdkVersion() >= 23)
        remote_selector->startDiscovery(QBluetoothUuid(reverseUuid));
    else
        remote_selector->startDiscovery(QBluetoothUuid(serviceUuid));
#else
    remote_selector->startDiscovery(QBluetoothUuid(serviceUuid));

    connect(remote_selector, &BluetoothScanner::sendAddService, this, &BluetoothManager::onServiceDiscovered);
#endif
}

BluetoothManager::~BluetoothManager() {
    bluetooth_client->deleteLater();
    bluetooth_server->deleteLater();
}

void BluetoothManager::onStartVideotransmission(const QBluetoothAddress& bluetooth_address) {
    QMetaObject::invokeMethod(bluetooth_client, "killConnection", Qt::BlockingQueuedConnection);
    QMetaObject::invokeMethod(bluetooth_client, "connectToService", Q_ARG(QBluetoothAddress, bluetooth_address), Qt::QueuedConnection);
}

void BluetoothManager::onStopTransmitt(const QBluetoothAddress& bluetooth_address) {}

void BluetoothManager::onServiceDiscovered(const QBluetoothServiceInfo& service_info) {
    services[service_info.device().address()] = service_info;
    this->addItem(service_info.device().name());
}

void BluetoothManager::onConnectToBluetoothService(QBluetoothAddress bluetooth_address) {
    QBluetoothServiceInfo service = services[bluetooth_address];

    qDebug() << "Connecting to service: " << service.serviceName() << " on: " << service.device().name();

    QMetaObject::invokeMethod(bluetooth_client, "killConnection", Qt::QueuedConnection);
    QMetaObject::invokeMethod(bluetooth_client, "connectToService", Q_ARG(QBluetoothServiceInfo, service), Qt::QueuedConnection);
    qDebug() << "Connecting...";

    connect(bluetooth_client, &BluetoothClient::sendMessageReceived, this, &BluetoothManager::onShowMessage);
    qDebug() << "Start client";

    bluetooth_client->connectToService(service);
}

void BluetoothManager::onShowMessage(const QString& sender, const QString& message) {
    QByteArray image_data = QByteArray::fromBase64(message.toLatin1());
    QLabel* label = new QLabel(this);
    label->setPixmap(QPixmap(image_data));
    label->show();
}

// void BluetoothManager::onNewAdapterSelected() {
//    const int new_adapter_index = adapterFromUserSelection();
//    if (current_adapter_index != new_adapter_index) {
//        bluetooth_server->stopServer();
//        current_adapter_index = new_adapter_index;
//        const QBluetoothHostInfo info = local_adapters.at(current_adapter_index);
//        QBluetoothLocalDevice adapter(info.address());
//        adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
//        bluetooth_server->initServer(info.address());
//        local_name = info.name();
//    }
//}

// int BluetoothManager::adapterFromUserSelection() const {
//    int result = 1;
//    QBluetoothAddress new_adapter = local_adapters.at(0).address();
//    return result;
//}
