#include "manager.h"

#include "client.h"
#include "remoteselector.h"
#include "server.h"

#include <QPixmap>
#include <QLabel>

#include <QtCore/qdebug.h>

#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

Manager::Manager(QWidget *parent)
    : QWidget(parent) {

    ui->setupUi(this);

    connect(/* твоя кнопка */, &QPushButton::clicked, this, &Manager::onConnectClicked);
    connect(/* твоя кнопка */, &QPushButton::clicked, this, &Manager::onSendStarted);

    local_adapters = QBluetoothLocalDevice::allDevices();
    if (local_adapters.count() < 2) {
        qDebug() << "localAdapters 0";
    } else {
        QBluetoothLocalDevice adapter(local_adapters.at(0).address());
        adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    }

    server = new Server(this);
    connect(server, QOverload<const QString &>::of(&Server::sendClientConnected),
            this, &Manager::onClientConnected);
    connect(server, QOverload<const QString &>::of(&Server::sendClientDisconnected),
            this,  QOverload<const QString &>::of(&Manager::onClientDisconnected));
    connect(server, &Server::sendMessageReceived,
            this,  &Manager::onShowMessage);
    connect(this, &Manager::sendMessage, server, &Server::onSendMessage);
    server->startServer();

    local_name = QBluetoothLocalDevice().name();
    timer = nullptr;
    label = nullptr;
}

Manager::~Manager() {
    qDeleteAll(clients);
    if(timer != nullptr) {
        delete timer;
    }
    if(label != nullptr) {
        delete label;
    }
    delete server;
}

void Manager::onClientConnected(const QString &name) {
    qDebug() << name << "connect";
}

void Manager::onClientDisconnected(const QString &name) {
    qDebug() << name << "disconnect";
}

void Manager::onConnected(const QString &name) {
    qDebug() << name << "connect";
}

void Manager::onNewAdapterSelected() {
    const int new_adapter_index = adapterFromUserSelection();
    if (current_adapter_index != new_adapter_index) {
        server->stopServer();
        current_adapter_index = new_adapter_index;
        const QBluetoothHostInfo info = local_adapters.at(current_adapter_index);
        QBluetoothLocalDevice adapter(info.address());
        adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        server->startServer(info.address());
        local_name = info.name();
    }
}

int Manager::adapterFromUserSelection() const {
    int result = 1;
    QBluetoothAddress new_adapter = local_adapters.at(0).address();
    return result;
}

void Manager::onReactOnSocketError(const QString &error) {
    qDebug() << error;
}

void Manager::onClientDisconnected() {
    Client *client = qobject_cast<Client *>(sender());
    if (client) {
        clients.removeOne(client);
        client->deleteLater();
    }
}

void Manager::onConnectClicked() {
    const QBluetoothAddress adapter = local_adapters.isEmpty() ? QBluetoothAddress() : local_adapters.at(current_adapter_index).address();

    RemoteSelector remote_selector(adapter);
#ifdef Q_OS_ANDROID
    if (QtAndroid::androidSdkVersion() >= 23)
        remote_selector.startDiscovery(QBluetoothUuid(reverseUuid));
    else
        remote_selector.startDiscovery(QBluetoothUuid(serviceUuid));
#else
    remote_selector.startDiscovery(QBluetoothUuid(serviceUuid));
#endif
    if (remote_selector.exec() == QDialog::Accepted) {
        QBluetoothServiceInfo service = remote_selector.service();

        qDebug() << "Connecting to service 2" << service.serviceName()
                 << "on" << service.device().name();

        qDebug() << "Going to create client";
        Client *client = new Client(this);
        qDebug() << "Connecting...";

        connect(client, &Client::sendMessageReceived,
                this, &Manager::onShowMessage);
        connect(client, &Client::sendDisconnected,
                this, QOverload<>::of(&Manager::onClientDisconnected));
        connect(client, QOverload<const QString &>::of(&Client::sendConnected),
                this, &Manager::onConnected);
        connect(client, &Client::sendSocketErrorOccurred,
                this, &Manager::onReactOnSocketError);
        connect(this, &Manager::sendMessage, client, &Client::onSendMessage);
        qDebug() << "Start client";
        client->startClient(service);

        clients.append(client);
    }

    ui->connectButton->setEnabled(true);
}

void Manager::onSendStarted()
{
    timer = new QTimer(this);
    timer->start(5000);
    connect(timer, &QTimer::timeout, this, [this]() {
        captureScreen();
    });
}

void Manager::onSendStopped()
{

}

void Manager::onShowMessage(const QString &sender, const QString &message)
{
    delete img;
    img = nullptr;
    QString data = QString::fromLatin1("%1").arg(message);
    QByteArray test = QByteArray::fromBase64(message.toLatin1());
    img = new BluetoothFormatImage(test);
    QPixmap pixmap;
    pixmap.loadFromData(test);
    label = new QLabel(this);
    label->setPixmap(pixmap);
    label->show();
}

void Manager::captureScreen() {
    QPixmap pixmap;
    delete img;
    img = nullptr;
    pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    img = new BluetoothFormatImage(pixmap);
    QByteArray send = img->getDataImage();
    QString message = send.toBase64() + '\n';
    emit sendMessage(message);
}

BluetoothFormatImage* Manager::getImg(){
    return img;
}

void Manager::setImg(){
    captureScreen();
}

